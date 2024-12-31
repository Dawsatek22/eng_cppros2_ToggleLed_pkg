/*this is a test service node to toggle a led with the client Node in ros2 jazzy */
// the terminal name : srv
// then comes the standard library
#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"
#include <memory>

// this node uses the wiringPi gpio library to control the led.
// for more info: https://github.com/WiringPi/WiringPi
#include <wiringPi.h> // is the gpio library
#include <softPwm.h> // makes pwm possible


int led1 = 16;
int led2 = 21;
void add(const std::shared_ptr<example_interfaces::srv::AddTwoInts::Request> request,
          std::shared_ptr<example_interfaces::srv::AddTwoInts::Response>      response)
{
    wiringPiSetupGpio(); // this functions setups the bcm pin numbering for use
    // now creates pwm values.
    softPwmCreate(led1,0,200);
    softPwmCreate(led2,0,200);
  response->sum = request->a + request->b;
  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Incoming request\na: %ld" " b: %ld",
                request->a, request->b);
  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "sending back response: [%ld]", (long int)response->sum);
softPwmWrite(led1,request->a ); // sets led1  value to equal to request a value
softPwmWrite(led2,request->b );  // sets led1  value to equal to request a value
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("add_two_ints_server");

  rclcpp::Service<example_interfaces::srv::AddTwoInts>::SharedPtr service =
    node->create_service<example_interfaces::srv::AddTwoInts>("add_two_ints", &add);

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ready to add two ints.");

  rclcpp::spin(node);
  rclcpp::shutdown();
}