//Для запуска программы и запуска тестов
#include <iostream>
//#include "time.h"
//#include "identifiable.h"
#include "works.h"
#include "resources.h"
#include "contractor.h"

#include "Time_test.h"
#include "identifiable_test.h"
#include "works_test.h"
#include "interval_test.h"
#include "resources_test.h"
#include "contractor_test.h"

using namespace std;

//int test_time(void) {
//	int test_num1 = 5, test_num2 = 1, tmp1, tmp2;;
//
//	Time t1(test_num1);
//	Time t2(test_num2);
//	bool sr1;
//
//	if (t1.get_val() != test_num1) {
//		cout << "t1 enable error\t";
//		return 0;
//	}
//	if (t1.get_val() != test_num1) {
//		cout << "t2 enable error\t";
//		return 0;
//	}
//
//	t1 = t1 + t2;
//
//	if (t1.get_val() != (test_num1 + test_num2)) {
//		cout << "sum error\t";
//		return 0;
//	}
//
//	tmp1 = t1.get_val();
//	tmp2 = t2.get_val();
//
//	t1 = t1 * t2;
//
//	if (t1.get_val() != (tmp1 * tmp2)) {
//		cout << "prosv error\t";
//		return 0;
//	}
//
//	sr1 = t1 < t2;
//	if (sr1) {
//		cout << "oper less error\t";
//		return 0;
//	}
//
//	return 1;
//}

//int test_idenf(void) {
//	string test_str = "kek1";
//
//	Identifiable t1;
//
//	t1.id = test_str;
//
//	if (t1.get_id() != test_str) {
//		cout << "idenf error\t";
//		return 0;
//	}
//
//	return 1;
//}

//int test_WorkerReq(void) {
//	string test_str[] = { "kek1", "kek2" };
//	Time time_t1(5);
//
//	WorkerReq t1(test_str[0], time_t1, 3, 5);
//
//	if (t1.get_kind() != test_str[0]) {
//		cout << "string test_str error\t";
//		return 0;
//	}
//	if (!(t1.get_volume() == time_t1)) {
//		cout << "get volume error\t";
//		return 0;
//	}
//	if (t1.get_max_count() != 5) {
//		cout << "max count error\t";
//		return 0;
//	}
//	if (t1.get_min_count() != 3) {
//		cout << "max count error\t";
//		return 0;
//	}
//
//	return 1;
//}
//
//int test_WorkUnit(void) {
//	string id_name = "name_1";
//	string test_str[] = { "kek1", "kek2" };
//	Time time_t1(5);
//	int volume1 = 1, volume2 = 2, volume3 = 3;
//	bool isServiceUnit1 = true, isServiceUnit2 = true, isServiceUnit3 = true;
//
//	WorkerReq w1(test_str[0], time_t1, 3, 5), w2(test_str[1], time_t1, 3, 5);
//	vector<WorkerReq> w = { w1, w2 };
//
//	WorkUnit t1(w, volume1, isServiceUnit1);
//
//	t1.id = id_name;
//
//	if (t1.get_id() != id_name) {
//		cout << "string test_str error\t";
//		return 0;
//	}
//	if (t1.get_volume() != volume1) {
//		cout << "get volume error\t";
//		return 0;
//	}
//	if (t1.get_isServiceUnit() != isServiceUnit1) {
//		cout << "get isServiceUnit1 error\t";
//		return 0;
//	}
//	if (t1.get_worker_reqs()[0].get_kind() != "kek1") {
//		cout << "max count error\t";
//		return 0;
//	}
//	if (t1.get_worker_reqs()[1].get_max_count() != 5) {
//		cout << "max count error\t";
//		return 0;
//	}
//
//	return 1;
//}

//int test_IntervalGaussian(void) {
//	int min_val = 1, max_val = 10;
//	IntervalGaussian inter1(5, 2, min_val, max_val);
//	float val_fl = inter1.randFloat();
//	int val_int = inter1.randInt();
//
//	//cout << inter1.randFloat() << endl;
//	if ((val_fl < min_val) && (val_fl > max_val))
//		return 0;
//	if ((val_int < min_val) && (val_int > max_val))
//		return 0;
//	
//	return 1;
//}

//int test_Worker(void) {
//	int min_val = 1, max_val = 10;
//	IntervalGaussian inter2(5, 2, min_val, max_val);
//	string w_id = "fd", w_name = "John", w_con_id = "kek";
//	int w_count = 3;
//	IntervalGaussian inter1(5, 2, min_val, max_val);
//	Worker w1(w_id, w_name, w_count, w_con_id, inter2);
//
//	if (w1.get_id() != w_id) {
//		cout << "string id error\t";
//		return 0;
//	}
//	if (w1.get_count() != w_count) {
//		cout << "get count error\t";
//		return 0;
//	}
//	if (w1.get_name() != w_name) {
//		cout << "get name error\t";
//		return 0;
//	}
//	if (w1.get_contractor_id() != w_con_id) {
//		cout << "get contractor id error\t";
//		return 0;
//	}
//	float prod1_fl = w1.get_productivity().randFloat();
//	int prod1_int = w1.get_productivity().randInt();
//
//	if ((prod1_fl < min_val) && (prod1_fl > max_val)) {
//		cout << "interval float error\t";
//		return 0;
//	}
//	if ((prod1_int < min_val) && (prod1_int > max_val)) {
//		cout << "interval int error\t";
//		return 0;
//	}
//
//	return 1;
//}

//int test_Contractor(void) {
//	int min_val = 1, max_val = 10;
//	IntervalGaussian inter2(5, 2, min_val, max_val);
//	string w_id[] = { "fd_1", "fd_2" }, w_name[] = { "John", "Max" }, w_con_id[] = { "kek1", "kek2" };
//	int w_count_1 = 3, w_count_2 = 3;
//	IntervalGaussian inter1(5, 2, min_val, max_val);
//	Worker w1(w_id[0], w_name[0], w_count_1, w_con_id[0], inter1), w2(w_id[1], w_name[1], w_count_1, w_con_id[1], inter2);
//	vector<Worker*> w = { &w1, &w2 };
//	Contractor c1(w);
//	c1.id = "idi_kek";
//
//	if (c1.get_id() != c1.id) {
//		cout << "string contractor id error\t";
//		return 0;
//	}
//	//cout << "c1 worker: " << c1.workers[0] << endl;
//
//	return 1;
//}

//int test_sort_WorkUnit(void) {
//	string id_name = "name_1";
//	string test_str[] = { "kek1", "kek2" };
//	Time time_t1(5);
//	int volume1 = 1, volume2 = 2, volume3 = 3;
//	bool isServiceUnit1 = true, isServiceUnit2 = true, isServiceUnit3 = true;
//
//	WorkerReq w1(test_str[0], time_t1, 3, 5), w2(test_str[1], time_t1, 3, 5);
//	vector<WorkerReq> w = { w1, w2 };
//
//	WorkUnit t1(w, volume1, isServiceUnit1);
//
//	t1.id = id_name;
//
//	if (t1.get_id() != id_name) {
//		cout << "string test_str error";
//		return 0;
//	}
//	if (t1.get_volume() != volume1) {
//		cout << "get volume error";
//		return 0;
//	}
//	if (t1.get_isServiceUnit() != isServiceUnit1) {
//		cout << "get isServiceUnit1 error";
//		return 0;
//	}
//	if (t1.get_worker_reqs()[0].get_kind() != "kek1") {
//		cout << "max count error";
//		return 0;
//	}
//	if (t1.get_worker_reqs()[1].get_max_count() != 5) {
//		cout << "max count error";
//		return 0;
//	}
//
//	return 1;
//}

void test_comand()
{
	std::cout << "program is working\n" << std::endl;

	cout << "initialization tests" << endl;

	cout << "test #1 ";
	if (test_time())
		cout << "ok" << endl;
	else
		cout << "fail" << endl;

	cout << "test #2 ";
	if (test_idenf())
		cout << "ok" << endl;
	else
		cout << "fail" << endl;

	cout << "test #3 ";
	if (test_WorkerReq())
		cout << "ok" << endl;
	else
		cout << "fail" << endl;

	cout << "test #4 ";
	if (test_WorkUnit())
		cout << "ok" << endl;
	else
		cout << "fail" << endl;

	cout << "test #5 ";
	if (test_IntervalGaussian())
		cout << "ok" << endl;
	else
		cout << "fail" << endl;

	cout << "test #6 ";
	if (test_Worker())
		cout << "ok" << endl;
	else
		cout << "fail" << endl;

	cout << "test #7 ";
	if (test_Contractor())
		cout << "ok" << endl;
	else
		cout << "fail" << endl;
}

int main()
{
	cout << "program is working" << endl;
	//test_comand();


	//cout << "\nsort tests" << endl;

	return 0;
}