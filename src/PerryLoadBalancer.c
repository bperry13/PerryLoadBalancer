/**
 * A file containing a set of functions to simulate a load balancer.
 *
 * @author: Brett Perry
 * @version: 2.14.22
 */
#include <stdio.h>
#include <stdlib.h>
#include "../hdr/LoadBalancer.h"
#include <pthread.h>

typedef struct balancer {
    int size;
    int requests;
    struct job_node* head;
} balancer;

/**
 * Initializes the load balancer. Takes batch size as parameter.
 */
balancer* balancer_create(int batch_size) {
    balancer *list = malloc(sizeof(struct balancer));
    list->size = batch_size;
    list->requests = 0;
    list->head = NULL;
    return list;
}

/**
 * Shuts down the load balancer. Ensures any outstanding batches have
 * completed.
 */
void balancer_destroy(balancer** lb) {
    host *host = host_create();
    balancer *b = *lb;
    if (b->head != NULL)
        host_request_instance(host, b->head);
    free(*lb);
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

    printf("LoadBalancer: Received new job from user #%d to process data=%d and store it at %p.\n", user_id, data, data_return);
    job_node *new_node = malloc(sizeof(struct job_node));
    new_node->user_id = user_id;
    new_node->data = data;
    new_node->data_result = data_return;

    //add node to the front
    //point new node to next in job list
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&lock);
    lb->requests++;
    new_node->next = lb->head;
    //assign new node to front of job list
    lb->head = new_node;
    pthread_mutex_unlock(&lock);
    if (lb->requests == lb->size) {
        lb->requests = 0;
        host *host = host_create();
        host_request_instance(host, lb->head);
    }
    pthread_mutex_destroy(&lock);
}