#include <minix/drivers.h>
#include <minix/chardriver.h>
#include <stdio.h>
#include <stdlib.h>
#include <minix/ds.h>
#include "hello.h"

/* structure of a stack node */
struct sNode
{
    int data;
    struct sNode *next;
};

/* Function to push an item to stack*/
void push(struct sNode** top_ref, int new_data);

/* Function to pop an item from stack*/
int pop(struct sNode** top_ref);

/* structure of queue having two stacks */
struct queue
{
    struct sNode *stack1;
    struct sNode *stack2;
};

/* Function to enqueue an item to queue */
void enQueue(struct queue *q, int x)
{
    push(&q->stack1, x);
}

/* Function to dequeue an item from queue */
int deQueue(struct queue *q)
{
    int x;
    /* If both stacks are empty then error */
    if(q->stack1 == NULL && q->stack2 == NULL)
    {
        printf("Q is empty");
        getchar();
        exit(0);
    }
    
    /* Move elements from stack1 to stack 2 only if
     stack2 is empty */
    if(q->stack2 == NULL)
    {
        while(q->stack1 != NULL)
        {
            x = pop(&q->stack1);
            push(&q->stack2, x);
            
        }
    }
    
    x = pop(&q->stack2);
    return x;
}

/* Function to push an item to stack*/
void push(struct sNode** top_ref, int new_data)
{
    /* allocate node */
    struct sNode* new_node =
    (struct sNode*) malloc(sizeof(struct sNode));
    if(new_node == NULL)
    {
        printf("Stack overflow \n");
        getchar();
        exit(0);
    }
    
    /* put in the data */
    new_node->data = new_data;
    
    /* link the old list off the new node */
    new_node->next = (*top_ref);
    
    /* move the head to point to the new node */
    (*top_ref) = new_node;
}

/* Function to pop an item from stack*/
int pop(struct sNode** top_ref)
{
    int res;
    struct sNode *top;
    
    /*If stack is empty then error */
    if(*top_ref == NULL)
    {
        printf("Stack underflow \n");
        getchar();
        exit(0);
        
    }
    else
    {
        top = *top_ref;
        res = top->data;
        *top_ref = top->next;
        free(top);
        return res;
        
    }
}

void fill_queue(int max_idx) {
    int i;
    for (i = 0; i < max_idx; ++i) {
        enQueue(q, i);
    }
}

/*
 * Function prototypes for the hello driver.
 */
static int hello_open(devminor_t minor, int access, endpoint_t user_endpt);
static int hello_close(devminor_t minor);
static ssize_t hello_read(devminor_t minor, u64_t position, endpoint_t endpt,
    cp_grant_id_t grant, size_t size, int flags, cdev_id_t id);

/* SEF functions and variables. */
static void sef_local_startup(void);
static int sef_cb_init(int type, sef_init_info_t *info);
static int sef_cb_lu_state_save(int);
static int lu_state_restore(void);

/* Entry points to the hello driver. */
static struct chardriver hello_tab =
{
    .cdr_open	= clipboard_open,
    .cdr_close	= clipboard_close,
    .cdr_read	= clipboard_read,
    .cdr_write   = clipboard_write,
};

typedef struct Registration{
    char* buffer;
    size_t len;
} reg;

int free_ids;
int* captured_regs;
reg* regs[100];
struct queue *q;
q->stack1 = NULL;

static int clipboard_open(devminor_t UNUSED(minor), int UNUSED(access),
    endpoint_t UNUSED(user_endpt))
{
    printf("hello_open()\n");
    return OK;
}

static int clipboard_close(devminor_t UNUSED(minor))
{
    printf("hello_close()\n");
    return OK;
}

static ssize_t clipboard_read(devminor_t UNUSED(minor), u64_t UNUSED(position),
    endpoint_t endpt, cp_grant_id_t grant, size_t size, int UNUSED(flags),
    cdev_id_t UNUSED(id))
{
    int ret;
    int id = (int) size;
    
    /* id out of range */
    if (id < 0 || id >= MAX_REGS)
        return -1;
    
    /* there is no reg with given id */
    if (!captured_regs[id])
        return -1;
    
    size_t len = regs[id]->len;
    char *text = calloc(len, sizeof(char));
    strcpy(text, regs[id]->buffer);
    free(regs[id]->buffer);
    free(regs[id]);
    enQueue(q, id);

    /* Copy the requested part to the caller. */
    if ((ret = sys_safecopyto(endpt, grant, 0, (vir_bytes) text, len)) != OK){
        free(text);
        return ret;
    }

    /* Return the number of bytes read. */
    return len;
}

static ssize_t clipboard_write(devminor_t UNUSED(minor), u64_t UNUSED(position),
                          endpoint_t endpt, cp_grant_id_t grant, size_t size, int UNUSED(flags),
                          cdev_id_t UNUSED(id))
{
    int id = deQueue(q);
    char* buffer = calloc(size, sizeof(char));
    if ((ret = sys_safecopyto(endpt, grant, 0, (vir_bytes) buffer, size)) != OK) {
        free(buffer);
        return ret;
    }
    
    regs[id] = calloc(1, sizeof(reg));
    regs[id]->buffer = calloc(size, sizeof(char));
    strcpy(regs[id]->buffer, buffer);
    free(buffer);
    
    /* Return the number of bytes read. */
    return size;
}

static int sef_cb_lu_state_save(int UNUSED(state)) {
/* Save the state. */
    ds_publish_u32("open_counter", open_counter, DSF_OVERWRITE);

    return OK;
}

static int lu_state_restore() {
/* Restore the state. */
    u32_t value;

    ds_retrieve_u32("open_counter", &value);
    ds_delete_u32("open_counter");
    open_counter = (int) value;

    return OK;
}

static void sef_local_startup()
{
    /*
     * Register init callbacks. Use the same function for all event types
     */
    sef_setcb_init_fresh(sef_cb_init);
    sef_setcb_init_lu(sef_cb_init);
    sef_setcb_init_restart(sef_cb_init);

    /*
     * Register live update callbacks.
     */
    /* - Agree to update immediately when LU is requested in a valid state. */
    sef_setcb_lu_prepare(sef_cb_lu_prepare_always_ready);
    /* - Support live update starting from any standard state. */
    sef_setcb_lu_state_isvalid(sef_cb_lu_state_isvalid_standard);
    /* - Register a custom routine to save the state. */
    sef_setcb_lu_state_save(sef_cb_lu_state_save);

    /* Let SEF perform startup. */
    sef_startup();
}

static int sef_cb_init(int type, sef_init_info_t *UNUSED(info))
{
/* Initialize the hello driver. */
    int do_announce_driver = TRUE;

    open_counter = 0;
    switch(type) {
        case SEF_INIT_FRESH:
            q = (struct queue*)malloc(sizeof(struct queue));
            fill_queue(MAX_REGS);
            free_ids = MAX_REGS;
        break;

        case SEF_INIT_LU:
            /* Restore the state. */
            lu_state_restore();
            do_announce_driver = FALSE;

            printf("%sHey, I'm a new version!\n", HELLO_MESSAGE);
        break;

        case SEF_INIT_RESTART:
            printf("%sHey, I've just been restarted!\n", HELLO_MESSAGE);
        break;
    }

    /* Announce we are up when necessary. */
    if (do_announce_driver) {
        chardriver_announce();
    }

    /* Initialization completed successfully. */
    return OK;
}

int main(void)
{
    /*
     * Perform initialization.
     */
    sef_local_startup();

    /*
     * Run the main loop.
     */
    chardriver_task(&hello_tab);
    return OK;
}
