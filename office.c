#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct employee {
	char * name;
	struct employee * subs;
	size_t n_subs;
}employee;

typedef struct office {
	struct employee * head;
}office;

typedef struct emp_lvl{
  employee * emp;
  char lvl;
}emp_lvl;

typedef struct queue {
	unsigned int size;
  size_t capacity;
	emp_lvl * emp_lvl;
} queue;

void que_tree(employee*, queue*, char);
queue* create_queue(size_t);
employee* search_node(employee*, employee*);

employee* search_tree(office* off, employee * emp){
  if(!off) return NULL;
  if(off->head->name == emp->name){
    return off->head;
  }
  if(!off->head->subs) return NULL;
  employee * node = off->head;
  for (int i = 0; i < node->n_subs; i++ ){
    employee* tmp;
    tmp = search_node(&node->subs[i], emp);
    if(tmp) return tmp;
  }
  return NULL;
}

employee* search_node(employee * emp, employee * target){
  if(emp->name == target->name){
    return emp;
  }
  if(!emp->subs){
    return NULL;
  }
  employee * node = emp;
  for(int i = 0; i < node->n_subs; i++){
    employee* tmp;
    tmp = search_node(&node->subs[i], target);
    if(tmp) return tmp;
  }
  return NULL;
}

queue* create_queue( size_t capacity ){
  queue* que = malloc(sizeof(queue));
	que->capacity = capacity;
	que->size = 0;
	que->emp_lvl = malloc(capacity*sizeof(emp_lvl));

  return que;
}

void enqueue (queue* que, employee * emp, char lvl){
  printf("Q-size: %d, insert %s at lvl: %d\n", que->size, emp->name, lvl);
  emp_lvl* emp_lvl = malloc(sizeof(emp_lvl));
  emp_lvl->emp = emp;
  emp_lvl->lvl = lvl;
  que->emp_lvl[que->size] = *emp_lvl;
	que->size++;
	return;
}

emp_lvl * dequeue(queue* que){
	if(que->size){
		que->size--;
		return &que->emp_lvl[que->size+1];
	}
	return NULL;
}

queue* populate_queue(employee * node){
	queue * que = create_queue(1000);
	que_tree(node, que, 0);
	return que;
}

void que_tree(employee * node, queue* que, char lvl){
	if(node){
		enqueue(que, node, lvl);
    for (int i = 0; i< node->n_subs; i++){
      que_tree(&node->subs[i], que, lvl+1);
		}
	}
}

employee * find_leaf(queue * que){
  emp_lvl *leaf = NULL;
  for(int i = 0; i < que->size; i++){
    employee *emp = que->emp_lvl[i].emp;
    char lvl = que->emp_lvl[i].lvl;
    if(!leaf && !emp->subs) {
      leaf = &que->emp_lvl[i];
    } else if (leaf && (lvl < leaf->lvl) && !emp->subs){
      leaf = &que->emp_lvl[i];
    } else {
      continue;
    }
	}
  return leaf->emp;
}

void traverse_queue(queue* que){
  employee* leaf = find_leaf(que);
	for(int i = 0; i < que->size; i++){
    if(que->emp_lvl[i].emp == leaf){
      printf("EMPLOYEE: %10s Level: %2d <<<< LEAF\n", que->emp_lvl[i].emp->name, que->emp_lvl[i].lvl);
    } else {
      printf("EMPLOYEE: %10s Level: %2d\n", que->emp_lvl[i].emp->name, que->emp_lvl[i].lvl);
    }
	}
}

void office_place(struct office* off, struct employee* emp, struct employee* sup){
	if(!emp) return;
  if(!off) return;
  employee * supr = search_tree(off, sup);
  if(!supr) return;
  supr->subs = realloc(supr->subs, (supr->n_subs + 1) * sizeof(employee));
  supr->subs[supr->n_subs] = *emp ;
  supr->n_subs++;
  printf("Adding: %s under %s, subs: %zu. addr_subs: %p, addr_sup: %p \n",emp->name,
            supr->name, supr->n_subs, supr->subs, supr);

}


int main()
{	
	struct employee jeff = {"Jeff", NULL, 0};
	struct employee bob = {"BOB", NULL, 0};
	struct employee vlad = {"Vlad", NULL, 0};
	struct employee turner = {"Turner", NULL, 0};
	struct employee tanner = {"Tanner", NULL, 0};
	struct employee sherlock = {"Sherlock", NULL, 0};


	office off;
	off.head = &jeff;

	office_place(&off,&bob,&jeff);
	office_place(&off,&vlad,&jeff);
	office_place(&off,&turner,&bob);
	office_place(&off,&tanner,&bob);
	office_place(&off,&sherlock,&tanner);

  //printf("%s, %p\n",jeffe->subs[0].name, jeffe->subs[0].subs );
  //printf("%p, %p\n", &jeffe->subs[0], bobe);
  /* for(int i = 0; i < bob.n_subs; i++){ */
    /* printf("Sub: %s\n", bob.subs[i].name); */
  /* } */
  queue* que = populate_queue(off.head);

  traverse_queue(que);

  free(que->emp_lvl);
  free(que);
  free(bob.subs);
  free(jeff.subs);
}



