#include "stdafx.h"
#include "CppUnitTest.h"
#include "GLFW_EngineCore.h"
#include "DebugLog.h"
#include "CameraComponent.h"
#include "FirstPersonCameraComponent.h"
#include "Game.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;



namespace EngineTests
{		
	TEST_CLASS(DebugLogTest)
	{
	public:
		
		TEST_METHOD(Log)
		{
			DebugLog& log = log.getInstance();

			std::string message = "in";

			log.writeLog("in");

			std::string output = log.v_messages[0];
			Assert::AreEqual(message, output);
		}

		
	};

	TEST_CLASS(CameraTests)
	{
	public:
		TEST_METHOD(Translate)
		{
			TransformComponent transform(glm::vec3(0.0f,0.0f,0.0f));
			
			transform.translate(0.0f, 5.0f, 0.0f);

			glm::vec3 expected_output = glm::vec3(0.0f, 5.0f, 0.0f);

			glm::vec3 actual_output = glm::vec3(transform.getPosition());

			Assert::AreEqual(expected_output.x, actual_output.x);
			Assert::AreEqual(expected_output.y, actual_output.y);
			Assert::AreEqual(expected_output.z, actual_output.z);

		}
		
		TEST_METHOD(cameraQuatRotate)
		{
			TransformComponent transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(1.0f,0.0f,0.0f,0.0f));

			transform.cameraQuatRotate(5.0f,0.0f);

			glm::quat expected_output = glm::quat(-0.801144f, 0.598474f, 0.0f, 0.0f);
		

			glm::quat actual_output = glm::quat(transform.getOrientation());

			
			Assert::AreEqual(expected_output.w , actual_output.w );
			Assert::AreEqual(expected_output.y, actual_output.y); // pass
			Assert::AreEqual(expected_output.z, actual_output.z); // pass
		    Assert::AreEqual(expected_output.x, actual_output.x);
		
			
			

		}

		TEST_METHOD(Rotate)
		{
			TransformComponent transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

			transform.rotate(5.0f, 1.0f,0.0f,0.0f);

			// used online euler angles to 3d quaternion calculator for this test
			glm::quat expected_output = glm::quat(-0.8011436f, 0.5984721f, 0.0f, 0.0f);


			glm::quat actual_output = glm::quat(transform.getOrientation());

			Assert::AreEqual(expected_output.w, actual_output.w);
			Assert::AreEqual(expected_output.x, actual_output.x);
			Assert::AreEqual(expected_output.y, actual_output.y);
			Assert::AreEqual(expected_output.z, actual_output.z);
			
		

		}
	};


}

