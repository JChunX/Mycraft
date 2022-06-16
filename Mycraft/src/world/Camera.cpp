#include"Camera.h"
#include"Shader.h"

Camera::Camera(int width, int height,
               float FOVdeg,
               float near_plane,
               float far_plane,
               glm::vec3 camera_position,
               glm::vec3 camera_orientation)
			    : WorldObject(camera_position, camera_orientation), 
				  InputListener()
{
    this->width = width;
    this->height = height;
    this->FOVdeg = FOVdeg;
    this->near_plane = near_plane;
    this->far_plane = far_plane;
}

void Camera::SetProjectionUniform(Shader& shader, const char* uniform)
{
    glm::mat4 projection = glm::perspective(glm::radians(FOVdeg), (float) width / (float) height, near_plane, far_plane);
	shader.SetMat4(uniform, projection);
}

void Camera::SetViewUniform(Shader& shader, const char* uniform)
{
    // calculate camera direction vector
    glm::vec3 direction = glm::normalize(glm::inverse(orientation) * glm::vec3(0.0f, 0.0f, -1.0f));
    // calculate camera target
    glm::vec3 target = position + direction;

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    // generate orthonormal basis
    glm::vec3 camera_right = glm::normalize(glm::cross(direction, up));
    glm::vec3 camera_up = glm::normalize(glm::cross(camera_right, direction));

    glm::mat4 view = glm::lookAt(position, target, camera_up);
	shader.SetMat4(uniform, view);
}

void Camera::SetProjectionViewUniforms(Shader& shader)
{
	SetProjectionUniform(shader, "projection");
	SetViewUniform(shader, "view");
}

void Camera::Input(GLFWwindow* window, UserInput& user_input, float delta_time)
{

	for (KeyInfo key_info : user_input.key_info)
	{
		if (key_info.key_action == PRESS)
		{
			glm::vec3 orientation_euler = GetEulerAngles();
			glm::vec3 delta;
			w_last_pressed = false;

			switch (key_info.key)
			{
			case GLFW_KEY_W:
				w_last_pressed = true;
				switch (motion_state)
				{
				case MotionState::SLOW:
				case MotionState::FAST:
					break;
				case MotionState::WAIT_FAST_ACTIVATION:
					if (glfwGetTime() - last_time < 0.3f)
					{
						motion_state = MotionState::FAST;
						forward_speed_multiplier = 3.0f;
					}
					else 
					{
						motion_state = MotionState::SLOW;
						forward_speed_multiplier = 1.0f;
					}
					break;
				}

			    delta = glm::vec3(glm::inverse(orientation) * glm::vec3(0.0f, 0.0f, -1.0f));
				delta.y = 0.0f;
				delta = glm::normalize(delta);
				position += delta * speed * forward_speed_multiplier;
				break;
			case GLFW_KEY_A:
				position += glm::vec3(glm::inverse(orientation) * glm::vec3(-speed, 0.0f, 0.0f));
				break;
			case GLFW_KEY_S:
				delta = glm::vec3(glm::inverse(orientation) * glm::vec3(0.0f, 0.0f, 1.0f));
				delta.y = 0.0f;
				delta = glm::normalize(delta);
				position += delta * speed;
				break;
			case GLFW_KEY_D:
				position += glm::vec3(glm::inverse(orientation) * glm::vec3(speed, 0.0f, 0.0f));
				break;
			case GLFW_KEY_SPACE:
        		position += glm::vec3(0.0f, speed, 0.0f);
				break;
			case GLFW_KEY_LEFT_SHIFT:
				position -= glm::vec3(0.0f, speed, 0.0f);
				break;
			default:
				break;
			}
		}
		if (key_info.key_action == RELEASE)
		{
			switch (key_info.key)
			{
			case GLFW_KEY_W:
				//position += glm::vec3(glm::inverse(orientation) * glm::vec3(0.0f, 0.0f, -1.0f)) * speed * forward_speed_multiplier * 1.0f;
				switch (motion_state)
				{
				case MotionState::SLOW:
					if (w_last_pressed)
					{
						motion_state = MotionState::WAIT_FAST_ACTIVATION;
						last_time = glfwGetTime();
						w_last_pressed = false;
					}
					break;
				case MotionState::FAST:
					motion_state = MotionState::SLOW;
					forward_speed_multiplier = 1.0f;
					break;
				case MotionState::WAIT_FAST_ACTIVATION:
					if (glfwGetTime() - last_time > 0.3f)
					{
						motion_state = MotionState::SLOW;
						forward_speed_multiplier = 1.0f;
					}
					break;
				}

			default:
				break;
			}
		}
	}
	for (MouseInfo mouse_info : user_input.mouse_info) {
		if (mouse_info.mouse_action == PRESS && mouse_info.mouse_button == GLFW_MOUSE_BUTTON_LEFT) {
			
			// Prevents camera from jumping on the first click
			if (first_click)
			{
				// Hides mouse cursor
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				std::cout << "First click" << std::endl;
				user_input.mouseX = width / 2;
				user_input.mouseY = height / 2;
				glfwSetCursorPos(window, user_input.mouseX, user_input.mouseY);
				first_click = false;
			}

		}
		else if (mouse_info.mouse_action == RELEASE && mouse_info.mouse_button == GLFW_MOUSE_BUTTON_LEFT && !first_click)
		{
			// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
			// and then "transforms" them into degrees 
			float rotX = sensitivity * (float)(user_input.mouseY - (height / 2)) / height;
			float rotY = sensitivity * (float)(user_input.mouseX - (width / 2)) / width;

			// get right vector given orientation quaternion
			glm::vec3 right = glm::normalize(glm::inverse(orientation) * glm::vec3(1.0f, 0.0f, 0.0f));

			// Rotate up and down
			glm::quat newOrientation = glm::rotate(orientation, glm::radians(-rotX), right);
			// Limit rotation below vertical
			if (glm::dot(newOrientation * glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) > 0.00f)
			{
				orientation = newOrientation;
			}

			// Rotates the Orientation left and right
			orientation = glm::rotate(orientation, glm::radians(-rotY), glm::vec3(0.0f, 1.0f, 0.0f));

			// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
			glfwSetCursorPos(window, (width / 2), (height / 2));
		}
	}
}