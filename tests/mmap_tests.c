#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 512

int main(int argc, char **argv)
{
	struct stat s;
	const char *file_name = "../ssh-key-2026-04-13.key.pub";
	int fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		perror("ERROR");
		return 1;
	}
	fstat(fd, &s);
	printf("file size in bytes: %zu", s.st_size);
	if (fd == -1)
		dprintf(1, "Failed to open file");
	void *buf = mmap(
			NULL, s.st_size, PROT_READ,
			MAP_PRIVATE, fd, 0
		);
	if ((long) buf == -1)
	{
		perror("ERROR");
		return 1;
	}
	printf("helllo\n");
	write(1, buf, s.st_size);



	char *str = "Hello, world!";
	void *mal = mmap(
			NULL, SIZE, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_HUGE_2MB | MAP_ANON,
			-1, 0
			);
	if ((long) mal == -1)
	{
		perror("ERROR");
		return 1;
	}
	strncpy(mal, str, strlen(str));
	printf("%s\n", (char *)mal);
	return (0);
}
