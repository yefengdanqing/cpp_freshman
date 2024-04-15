set(THIRD_PARTY_PREFIX ${CMAKE_BINARY_DIR}/third_party)
set(BOOST_ROOT ${THIRD_PARTY_PREFIX}/boost)
set(BOOST_INCLUDE_LIBRARIES thread filesystem system graph)
set(BOOST_ENABLE_CMAKE ON)
set(FETCHCONTENT_QUIET FALSE)

include(FetchContent)

FetchContent_Declare(
  boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG boost-1.81.0
  SOURCE_DIR ${BOOST_ROOT}
  BINARY_DIR ${BOOST_ROOT}
)
FetchContent_MakeAvailable(boost)
include_directories(${boost_SOURCE_DIR}/include)
# set(LIB_BIBRARY
#     ${LIB_BIBRARY}
#     "Boost::thread Boost::graph")
# set(LIB_DEPENDS
#          "${LIB_DEPENDS} Boost::thread Boost::graph")