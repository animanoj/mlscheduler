from random import randint

wfile = open("example.in", "w");

total_seconds = int(raw_input("Input total seconds of sleep: "));
upper_limit = int(raw_input("Input upper limit of sleep time: "));

while total_seconds >= upper_limit:
    r_num = randint(1, upper_limit - 1);
    wfile.write("sleep " + str(r_num) + "\n");
    total_seconds -= r_num;

wfile.write("sleep " + str(total_seconds) + "\n");

wfile.close();
