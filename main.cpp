#include <iostream>
#include <string>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

struct Args {
	char	*input_file;
	char	*output_file;
	int		block_size = 1;
};


int	read_file(const char *input_file)
{
	int fd = open(input_file, O_RDONLY);
	if (errno != 0) {
		fprintf(stderr, "errno is %d!\n", errno);
		return 0;
	}
//	Read file length
	off_t length = lseek(fd, 0, SEEK_END);
/* go to the location corresponding to the last byte */
	if (lseek(fd, length - 1, SEEK_SET) == -1)
	{printf ("lseek error");
		return 0;
	}
/* write a dummy byte at the last location */
	if (write (fd, "", 1) != 1)
	{printf ("write error");
		return 0;
	}
	void* memory_file = nullptr;
	if ((memory_file = mmap(nullptr, length, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0)) == (caddr_t)-1) {
		printf ("mmap error for output");
		fprintf(stderr, "errno is %d!\n", errno);
		return 0;
	}
	for(size_t i = 0; i < length; ++i) {
		((char*)memory_file)[i] = 'a';
	}
	munmap(memory_file, length);
	std::cout << "Hello, World!" << std::endl;
	return 0;
}

void	parse_args(int argc, char **argv, Args &input_args)
{
	if (argc < 3)
		throw std::invalid_argument("Usage: a.out <fromFile> <toFile> <blockSize:optional>");
	input_args.input_file = argv[1];
	input_args.output_file = argv[2];
	if (argv[3])
	{
		input_args.block_size = atoi(argv[3]);
		if (input_args.block_size == 0)
			throw std::invalid_argument("Wrong block size");
	}
}

int		main(int argc, char **argv)
{
	Args	input_args;
//	cout << "lol";
	parse_args(argc, argv, input_args);
	return(read_file(input_args.input_file));
//	cout << "lole";

}
