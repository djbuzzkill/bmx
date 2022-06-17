#include "mtool.h"

#include <glm/glm.hpp>
#include <iostream>

#include <vector>
#include <string>

#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>
#include "System.h"


#include <zmq.h>

int main (int argv, char** argc)
{
  std::vector<std::string> args (argc, argc+argv);

  glm::vec3 v (2.1f, 2.2f, 2.3f);

  glm::vec3 x = v + v;
  
  std::cout << "wat:" << x[0] + args.size () << std::endl;


  auto wat = glm::dot(x, x);
  
  return 0; 
}
