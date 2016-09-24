/*
 * Copyright (c) 2016
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#define BOOST_TEST_MODULE POCO_TASK_MANAGER 
#include <boost/test/unit_test.hpp>

/*
* Objective
* Explore tht utility of POCO test manager classes for writing generic services.
*/

#include "Poco/Exception.h"
#include "Poco/TaskManager.h"
#include "Poco/Task.h"
#include "Poco/TaskNotification.h"
#include "Poco/NotificationCenter.h"
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"
#include "Poco/Event.h"
#include "Poco/Observer.h"
#include "Poco/Exception.h"
#include "Poco/AutoPtr.h"
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>

class UserTask : public Poco::Task {
    private:
        int m_id;
    public:
        UserTask()
            :Task("UserTask")
        {}
        void runTask()
        {Poco::Thread::sleep(10000);}
};

class TaskObserver{
    private:
        bool m_started;
        bool m_cancelled;
        bool m_finished;
    public:
        TaskObserver():
            m_started(false)
            ,m_cancelled(false)
            ,m_finished(false)
    {}
        void taskStarted(Poco::TaskStartedNotification* pNf){
            m_started = true;
            pNf->release();
        }

        void taskCancelled(Poco::TaskCancelledNotification* pNf){
            m_cancelled = true;
            pNf->release();
        }

        void taskFinished(Poco::TaskFinishedNotification* pNf){
            m_finished = true;
            pNf->release();
        }
		
        bool Started() const{
			return m_started;
		}
		
		bool Cancelled() const{
			return m_cancelled;
		}
		
		bool Finished() const{
			return m_finished;
		}
};

BOOST_AUTO_TEST_SUITE(poco_task_manager)
BOOST_AUTO_TEST_CASE(basic_manager){
    Poco::TaskManager test_tm;
    TaskObserver to;
	test_tm.addObserver(Poco::Observer<TaskObserver, Poco::TaskStartedNotification>(to, &TaskObserver::taskStarted));
	test_tm.addObserver(Poco::Observer<TaskObserver, Poco::TaskCancelledNotification>(to, &TaskObserver::taskCancelled));
	test_tm.addObserver(Poco::Observer<TaskObserver, Poco::TaskFinishedNotification>(to, &TaskObserver::taskFinished));
    // The task manager takes ownership of the allocated object hence not explicit delete necessary.
    // Using a POCO AutoPtr also perfoms the same delegation.
    // Hence, using boost::unique_ptr or std::unique_ptr will cause a SIGSEGV at the end of test case.
    Poco::AutoPtr<UserTask> usr_task = new UserTask;
    test_tm.start(usr_task); 
    Poco::Thread::sleep(10);
    BOOST_ASSERT(to.Started());
    while(!to.Finished()){
        Poco::Thread::sleep(1000);
        BOOST_CHECK(!to.Cancelled());
    }
    Poco::TaskManager::TaskList t_list = test_tm.taskList();
    BOOST_CHECK(t_list.empty());
}

BOOST_AUTO_TEST_CASE(manager_thread_pool){

    Poco::ThreadPool test_pool(2,3,5);
    test_pool.setStackSize(1);
    // This will allow controlling resources and reducing too many active threads.
    // Default value per task manager is 16 threads.
    Poco::TaskManager test_tm(test_pool);
    TaskObserver to;
	test_tm.addObserver(Poco::Observer<TaskObserver, Poco::TaskStartedNotification>(to, &TaskObserver::taskStarted));
	test_tm.addObserver(Poco::Observer<TaskObserver, Poco::TaskCancelledNotification>(to, &TaskObserver::taskCancelled));
	test_tm.addObserver(Poco::Observer<TaskObserver, Poco::TaskFinishedNotification>(to, &TaskObserver::taskFinished));

    try{
        test_tm.start(new UserTask); 
        test_tm.start(new UserTask); 
        test_tm.start(new UserTask); 
        test_tm.start(new UserTask); 
    }catch(Poco::NoThreadAvailableException& e){
        BOOST_TEST_MESSAGE( "[Exception] : " << e.what());
    }
    catch (...){
        BOOST_ASSERT( 0 && "unexpected exception type thrown.");
    }
    BOOST_CHECK(test_tm.count() == test_pool.allocated());
    test_tm.joinAll();
}
BOOST_AUTO_TEST_SUITE_END()



