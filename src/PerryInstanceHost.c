/**
 * When the load balancer in our system needs to create an instance, it will
 * simply called host_request_instance() from InstanceHost.c. It will not make
 * any sort of network connection.
 *
 * Completion time: 8 hours
 *
 * @author: Brett Perry
 * @version: 2.14.22
 */
#include "../hdr/InstanceHost.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct host {
    int num;
} host;

/**
* Initializes the host environment.
*/
host* host_create() {
    host *h = malloc(sizeof(host));
    h = NULL;
    return h;
}

/**
* Shuts down the host environment. Ensures any outstanding batches have
* completed.
*/
void host_destroy(host** h) {
    free(*h);
    h = NULL;
}

/**
* Creates a new server instance (i.e., thread) to handle processing the items
* contained in a batch (i.e., a listed list of job_node). InstanceHost will
* maintain a list of active instances, and if the host is requested to
* shutdown, ensures that all jobs are completed.
*
* @param job_batch_list A list containing the jobs in a batch to process.
*/
void host_request_instance(host* h, struct job_node* batch) {

    //while batch not equal to null
    while (batch != NULL) {
        int *result = batch->data_result;
        *result = batch->data * batch->data;
        batch = batch->next;
    }
}