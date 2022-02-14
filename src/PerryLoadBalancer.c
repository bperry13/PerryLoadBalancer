/**
 * A file containing a set of functions to simulate a load balancer.
 *
 * @author: Brett Perry
 * @version: 2.14.22
 */
#include <stdlib.h>
#include "../hdr/LoadBalancer.h"

/**
 * Initializes the load balancer. Takes batch size as parameter.
 */
balancer* balancer_create(int batch_size) {
    balancer *list = malloc(sizeof(struct balancer));
    list->size = batch_size;
    return list;
}

/**
 * Shuts down the load balancer. Ensures any outstanding batches have
 * completed.
 */
void balancer_destroy(balancer** lb) {
    free(lb);
}

/**
 * Adds a job to the load balancer. If enough jobs have been added to fill a
 * batch, will request a new instance from InstanceHost. When job is complete,
 * *data_return will be updated with the result.
 *
 * @param user_id the id of the user making the request.
 * @param data the data the user wants to process.
 * @param data_return a pointer to a location to store the result of processing.
 */
void balancer_add_job(balancer* lb, int user_id, int data, int* data_return) {

    job_node* new_node = malloc(sizeof(struct job_node));
    new_node->user_id = user_id;
    new_node->data = data;
    new_node->data_result = data_return;

    //add node to the front
    //point new node to next in job list
    lb->job_list->next = lb->job_list;
    //assign new node to front of job list
    lb->job_list = new_node;
}