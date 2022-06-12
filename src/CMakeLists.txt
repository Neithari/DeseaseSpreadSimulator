add_subdirectory(DiseaseSpreadSimulator)

#include(FetchContent)
#FetchContent_Declare(SFML
#    GIT_REPOSITORY https://github.com/SFML/SFML.git
#    GIT_TAG 2.6.x)
#FetchContent_MakeAvailable(SFML)

#target_link_libraries(CMakeSFMLProject PRIVATE sfml-graphics)

#find_package(imgui CONFIG REQUIRED)
#target_link_libraries(main PRIVATE imgui::imgui)

#find_package(ImGui-SFML CONFIG REQUIRED)
#target_link_libraries(main PRIVATE ImGui-SFML::ImGui-SFML)

#find_package(implot CONFIG REQUIRED)
#target_link_libraries(main PRIVATE implot::implot)
