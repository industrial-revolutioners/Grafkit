/*
TCs for internal classes such as scene and so on
*/

#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"

#include <gtest/gtest.h>

#include <cmath>

#include "render/Scene.h"
#include "render/Model.h"

#include "utils/persistence/dynamics.h"
#include "utils/persistence/persistence.h"

#include "TestArchiver.h"
#include "testClass_persistence.h"

using namespace Grafkit;
using namespace FWdebugExceptions;
using namespace ArchivePersistent;

#define DEFINE_PREGNANCY_TEST(CLASS) \
	TEST(Persistence_Pregnancy, given_##CLASS##_when_Persist_then_Load) {\
		TestArchiver archive(16 * 4096, true);\
\
		Ref<CLASS> object = new CLASS();\
		PERSIST_REFOBJECT(archive, object);\
\
		archive.ResetCrsr();\
		archive.SetDirection(false);\
\
		Ref<CLASS> object_read;\
		PERSIST_REFOBJECT(archive, object_read);\
\
		ASSERT_TRUE(object_read.Valid());\
		ASSERT_TRUE(object.Get() != object_read.Get());\
	}

DEFINE_PREGNANCY_TEST(Scene)
DEFINE_PREGNANCY_TEST(Actor)
DEFINE_PREGNANCY_TEST(Material)
DEFINE_PREGNANCY_TEST(Mesh)
DEFINE_PREGNANCY_TEST(Model)
