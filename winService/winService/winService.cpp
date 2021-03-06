// winService.cpp : Defines the entry point for the console application.
//

#include "TestService.h"

void show_help()
{
	printf("-i[|I] register the service\n");
	printf("-r[|R] remove the service\n");
	printf("-d[|D] run in debug mode\n\n");
	exit(0);
}

int main(int argc, char* argv[])
{
	if (argc < 1)
	{
		exit(0);
	}

	if (argc > 2)
	{
		show_help();
	}

	TestService service("myService", "TestService");

	if (argc == 2)
	{
		if (argv[1][0] != '-')
		{
			show_help();
		}

		switch (argv[1][1])
		{
		case 'i':
		case 'I':
			printf("register\n");
			service.RegisterService();
			break;
		case 'r':
		case 'R':
			printf("remove\n");
			service.RemoveService();
			break;
		case 'd':
		case 'D':
			printf("debug run\n");
			TestService::service_main_fun();
			break;
		default:
			show_help();
			break;
		}
	}
	else
	{
		service.RunService();
	}

	return 0;
}

