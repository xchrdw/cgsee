#pragma once

#include <glm/glm.hpp>

class ViewHistory
{

public:
	ViewHistory(ViewHistory* previous, glm::mat4 viewmatrix, float fovy);
	~ViewHistory();
	void reset();

	void setNext(ViewHistory* next);
	void setPrevious(ViewHistory* previous);
	ViewHistory* getPrevious();
	ViewHistory* getNext();
	ViewHistory* getLast();
	ViewHistory* getFirst();
	bool isLast();
	bool isFirst();
	bool isEqualTo(const glm::mat4 & viewmatrix);
	int getId();
	int getSize();

	glm::mat4 getViewMatrix();
	float getFovy();

private:

	void deleteOrphanedHistory();
	void deleteOldestHistory();

	static int m_size;
	int m_id;
	ViewHistory* m_next;
	ViewHistory* m_previous;
	float m_fovy;
	glm::mat4 m_viewmatrix;
	// small view screenshot m_thumbnail
	// timestamp
};
