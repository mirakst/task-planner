mainmake: main.c lib/calculate_prices.c lib/tasks.c lib/user_details.c
	gcc main.c lib/calculate_prices.c lib/tasks.c lib/user_details.c -I.