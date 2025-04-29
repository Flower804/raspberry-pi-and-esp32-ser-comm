# raspberry-pi-and-esp32-ser-comm <br>

This project is made by <b> Flower </b> for the [Tugaspace](https://github.com/TugaSpace) team in the <b> Cansat 2024 competition </b> <br><br>
This project uses [multithreading](https://www.freertos.org/Documentation/RTOS_book.html) on the esp32 to communicate with the Raspberry Pi via Serial <br><br>

This project has the objective of being able to be scalable and be aplied to diferent projects and diferent contextes. <br>
You can find the final code aplied to the Tugaspace's Cansat 2024 competition in the [Tugaspace_code_2024](https://github.com/Flower804/Tugaspace_code_2024/tree/main) repository
# FILES <br>
 - esp32_ser_alpha.ino: arduino code for the esp32 <br>
 - rasp_ser_beta.py: python code for the Raspberry Pi <br>

# Exclaimers
For all of this data transfer, the comunication betwheen the RaspBerry Pi and the esp32 is **handled via Serial**

# Explanation <br>
## esp32_ser_alpha <br>
This code uses [multithreading](https://www.freertos.org/Documentation/RTOS_book.html) to establish a data transfer connection by data confirmation, and receives data from the Raspberry Pi. <br>
 - **Thread 1**: HUB handle <br>
 - **Thread 2**: esp turn handle <br>

### Hub handle <br>
This thread serves as a controler, to verify if the Raspeberry Pi has finished receiving and processing the data, and once the Raspberry Pi is ready to receive more data resumes the execution of the **esp turn handle**.

### esp turn handle <br> 
This thread is responsible to send the data gathered to be sent to the Raspberry Pi. <br>
It first verifies if the Raspberry Pi has sent the signal anouncing that it has started the data receiving protocol and once it has started, starts sending the desired data in a *wait_time* interval. <br>
Once it ends sending the desidered data it waits for the Raspberry Pi to send a *done* code and enters the wait state(**HUB handle**). 

## Rasp_ser_beta <br>
This code is a simple data receiver protocol, where it writes the received data on a txt file. <br>
When the Rasberry Pi is ready to receive data it sends a "ready" signal, and reads the Serial until it finds a "received" signal where it writes the data to a txt file. <br>
It writes the data until finding a "send_complete" marker, where it saves the current state of the file and sends a "done" signal to the esp32, where it goes back to receiving Serial data. 

# Flower Notes :P
DAAAMMMMM THIS IS SO OLD, decided to look back at my older projects and decided to Archive a lot of them, but decided to write a proper README for this fun little project, and bro I really sucked at coding back then, but tbf this was made in 2/3 days if I remember because I was warned on top of time they needed this done, nothing compared to the hellhole creation I did during the competition, but it was interesting looking back at the code. <br>
That's that, good old memories
