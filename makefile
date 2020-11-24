mainmake: main.c lib/calculate_prices.c lib/tasks.c lib/user_details.c lib/graphics.c
	gcc main.c lib/calculate_prices.c lib/tasks.c lib/user_details.c lib/graphics.c -ansi -Wall -pedantic -I.