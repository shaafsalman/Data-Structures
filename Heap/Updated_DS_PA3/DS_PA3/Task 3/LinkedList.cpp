#ifndef __LIST_CPP
#define __LIST_CPP

#include <cstdlib>
#include <memory>
#include <iostream>
#include "LinkedList.h"
using namespace std;

template <class T>
LinkedList<T>::LinkedList()
{
    this->head = NULL;
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T> &otherLinkedList)
{
    shared_ptr<ListItem<T>> temp = otherLinkedList.head;
    if (otherLinkedList.head == NULL)
    {
        head = NULL;
    }
    else if (otherLinkedList.head->next == NULL)
    {
        head = temp;
    }
    else
    {
        head = NULL;
        while (temp != NULL)
        {
            insertAtTail(temp->value);
            temp = temp;
        }
    }
}

template <class T>
void LinkedList<T>::insertAtHead(T item)
{
    if (head == NULL)
    {
        shared_ptr<ListItem<T>> temp(new ListItem<T>(item));
        head = temp;
    }
    else
    {
        shared_ptr<ListItem<T>> newnode(new ListItem<T>(item));
        newnode->prev = NULL;
        newnode->next = head;
        head->prev = newnode;
        head = newnode;
    }

    return;
}

template <class T>
void LinkedList<T>::insertAtTail(T item)
{
    shared_ptr<ListItem<T>> new_item(new ListItem<T>(item));

    if (head == NULL)
    {
        head = new_item;
    }
    else
    {
        new_item->next = NULL;
        shared_ptr<ListItem<T>> tail = getTail();
        new_item->prev = tail;
        tail->next = new_item;
    }

    return;
}

template <class T>
void LinkedList<T>::insertAfter(T toInsert, T afterWhat)
{
    shared_ptr<ListItem<T>> current_node = head;
    shared_ptr<ListItem<T>> previousnode;

    if (head == NULL)
    {
        return;
    }
    else if ((head->value == afterWhat) && (head->next == NULL))
    {
        shared_ptr<ListItem<T>> newnode(new ListItem<T>(toInsert));
        newnode->next = NULL;
        newnode->prev = head;
        head->next = newnode;
    }
    else if (getTail()->value == afterWhat)
    {
        insertAtTail(toInsert);
    }
    else
    {
        while (current_node->next != NULL)
        {
            if (current_node->value == afterWhat)
            {
                previousnode = current_node;
                current_node = current_node->next;
                shared_ptr<ListItem<T>> newnode(new ListItem<T>(toInsert));
                previousnode->next = newnode;
                current_node->prev = newnode;
                newnode->next = current_node;
                newnode->prev = previousnode;
                break;
            }
            else
            {
                current_node = current_node->next;
            }
        }
    }

    return;
}

template <class T>
shared_ptr<ListItem<T>> LinkedList<T>::getHead()
{
    return head;
}

template <class T>
shared_ptr<ListItem<T>> LinkedList<T>::getTail()
{
    shared_ptr<ListItem<T>> current_node = head;
    if (current_node == NULL)
    {
        return NULL;
    }

    while (current_node->next != NULL)
    {
        current_node = current_node->next;
    }

    return current_node;
}

template <class T>
shared_ptr<ListItem<T>> LinkedList<T>::searchFor(T item)
{
    if (head == NULL)
    {
        return NULL;
    }
    else if (head->value == item)
    {
        return head;
    }
    else
    {
        shared_ptr<ListItem<T>> current_node = head;
        while (current_node != NULL)
        {
            if (current_node->value == item)
            {
                return current_node;
            }
            else
            {
                current_node = current_node->next;
            }
        }

        return NULL;
    }
}

template <class T>
void LinkedList<T>::deleteElement(T item)
{
    if (head == NULL)
    {
        return;
    }
    else if (head->value == item)
    {
        deleteHead();
    }
    else
    {
        shared_ptr<ListItem<T>> current_node = searchFor(item);
        if (current_node == NULL)
        {
            return;
        }
        else if (current_node->next == NULL)
        {
            current_node->prev->next = NULL;
            current_node->prev = NULL;
            current_node.reset();
            current_node = NULL;
        }
        else
        {
            shared_ptr<ListItem<T>> previousnode = current_node->prev;
            shared_ptr<ListItem<T>> nextnode = current_node->next;
            previousnode->next = nextnode;
            nextnode->prev = previousnode;
            current_node.reset();
            current_node = NULL;
            return;
        }
    }
}

template <class T>
void LinkedList<T>::deleteHead()
{
    shared_ptr<ListItem<T>> current_node = head;
    if (head == NULL)
    {
        return;
    }
    else if (head->next == NULL)
    {
        head.reset();
        head = NULL;
    }
    else
    {
        head = head->next;
        head->prev = NULL;
        current_node.reset();
        current_node = NULL;
    }

    return;
}

template <class T>
void LinkedList<T>::deleteTail()
{
    shared_ptr<ListItem<T>> tail = getTail();
    if (tail == NULL)
    {
        return;
    }
    else if (tail->prev == NULL)
    {
        head.reset();
        head = NULL;
    }
    else
    {
        tail->prev->next = NULL;
        tail->prev = NULL;
        tail.reset();
        tail = NULL;
    }

    return;
}

template <class T>
int LinkedList<T>::length()
{
    shared_ptr<ListItem<T>> current_node = head;

    if (head == NULL)
    {
        return 0;
    }
    else if (head->next == NULL)
    {
        return 1;
    }
    else
    {
        int counter = 0;
        while (current_node->next != NULL)
        {
            counter++;
            current_node = current_node->next;
        }
        counter++;
        return counter;
    }
}

#endif
