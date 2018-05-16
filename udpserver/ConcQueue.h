/*
 * ConcQueue.h
 *
 *  Created on: Mar 2, 2018
 *      Author: ducvo
 */

#ifndef CONCQUEUE_H_
#define CONCQUEUE_H_
#include <mutex>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

template < class T,
    class Container = std::vector<T>,
    class Compare = std::greater<typename Container::value_type> >
class ConcQueue {
private:
	mutex mLock;
	condition_variable cond;
	priority_queue<T, Container, Compare> q;

public:
	ConcQueue() {

	}

	virtual ~ConcQueue() {

	}

	T pop() {
		unique_lock<mutex> l(mLock);
		while(q.empty()) {
			cond.wait(l);
		}
		T v = q.top();
		q.pop();
		return v;
	}

	void push(const T& v) {
		unique_lock<mutex> l(mLock);
		q.push(v);
		l.unlock();
		cond.notify_one();
	}
};

#endif /* CONCQUEUE_H_ */
