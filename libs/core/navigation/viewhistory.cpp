#include "viewhistory.h"
#include <glm/glm.hpp>

int ViewHistory::m_size = 0;

ViewHistory::ViewHistory(ViewHistory* previous, glm::mat4 viewmatrix, float fovy)
	: m_previous(previous)
	, m_viewmatrix(viewmatrix)
	, m_fovy(fovy)
	, m_next(nullptr)
{
	m_id = m_size;
	m_size++;
	
	// todo! implement history size limit

	if(previous != nullptr){
		/*
		* if new history shall be saved before youngest state is the
		* the current view: forget younger history (create new path)
		*/
		if(previous->getNext() != nullptr){
			deleteYoungerHistory();
		}

		// connect previous node with current
		previous->setNext(this);
	} 
}

ViewHistory::~ViewHistory(){
	m_size--;
}

void ViewHistory::reset(){
	// todo!
}

void ViewHistory::setNext(ViewHistory* next){
	m_next = next;
}

void ViewHistory::setPrevious(ViewHistory* previous){
	m_previous = previous;
}

ViewHistory* ViewHistory::getPrevious(){
	return m_previous;
}

ViewHistory* ViewHistory::getNext(){
	return m_next;
}

ViewHistory* ViewHistory::getLast(){
	ViewHistory* temp {this};	
	while(!temp->isLast()){
		temp = temp->getNext();
	}
	return temp;
}

ViewHistory* ViewHistory::getFirst(){
	ViewHistory* temp {this};	
	while(!temp->isFirst()){
		temp = temp->getPrevious();
	}
	return temp;
}

void ViewHistory::deleteYoungerHistory(){
	ViewHistory* temp {m_previous->getLast()};	
	while(temp != m_previous){
		temp = temp->getPrevious();
		delete temp->getNext();
	}
}

glm::mat4 ViewHistory::getViewMatrix(){
	return m_viewmatrix;
}

float ViewHistory::getFovy(){
	return m_fovy;
}
	
int ViewHistory::getSize(){
	return m_size;
}

int ViewHistory::getId(){
	return m_id;
}

bool ViewHistory::isFirst(){
	if(m_previous==nullptr){
		return true;
	}else{
		return false;
	}
}

bool ViewHistory::isLast(){
	if(m_next==nullptr){
		return true;
	}else{
		return false;
	}
}