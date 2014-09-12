#ifndef __TEST_MSG_H
#define __TEST_MSG_H

enum msg_type{
	SET_DET = 1,
	SG
};

enum det_stat{
	RISING = 1,
	FALLING = 2,
	FAULT = 3
};

struct set_det_data{
	int id;
	enum det_stat stat;
};

struct test_msg{
	enum msg_type type;
	char data[1024];
};
#endif /*__TEST_MSG_H*/
