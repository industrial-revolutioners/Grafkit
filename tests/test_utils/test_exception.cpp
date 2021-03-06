#include "stdafx.h"

#include <cstring>

#include <gtest/gtest.h>
#include "core/exceptions.h"

using namespace FWdebugExceptions;

void throw_nullpointer_exception() {
	THROW_EX(NullPointerException);
}

TEST(Exception, given_ThrownException)
{
	EXPECT_THROW(throw_nullpointer_exception(), NullPointerException);
}

void throw_nullpointer_exception_details(const char *what) {
	THROW_EX_DETAILS(NullPointerException, what);
}

TEST(Exception, given_ThrownExceptionWithDetails)
{
	const char *message = "test message";
	try {
		throw_nullpointer_exception_details(message);
		FAIL();
	}
	catch (NullPointerException &e) {
		const char *result = e.what();
		//ASSERT_NE(NULL, result);

		const char *pch = strstr(result, message);

		//ASSERT_NE(NULL, pch);
		ASSERT_STREQ(message, pch);
	}
	catch (...) {
		FAIL();
	}
}
