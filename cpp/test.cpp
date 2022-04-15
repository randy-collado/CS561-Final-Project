//
// Created by Randy Collado on 4/14/22.
//

#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <vector>

struct test_s {
    std::vector<int> v;
};
int main(int argc, char** argv){
    int fd = open("test.test", O_RDWR | O_CREAT);

    test_s s{std::vector<int>{3, 6, 9}};

    test_s* s_out = new test_s();

    auto bytes = pwrite(fd, &s, sizeof(test_s), 0);

    bytes = pread(fd, s_out, sizeof(test_s), 0);

    std::cout << s_out->v[0] << " " << s_out->v[1] << std::endl;
}
