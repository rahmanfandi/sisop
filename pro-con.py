import threading
import time
import random
from threading import Semaphore

# Buffer size
BUFFER_SIZE = 5
buffer = [None] * BUFFER_SIZE
in_index = 0
out_index = 0
# Semaphore
empty = Semaphore(BUFFER_SIZE)  # Semaphore counting empty slots
full = Semaphore(0)  # Semaphore counting full slots
mutex = (
    threading.Lock()
)  # Mutex to ensure only one thread accesses the buffer at a time


# Producer function
def producer(producer_id):
    global in_index
    while True:
        item = random.randint(1, 100)  # Produce an item
        empty.acquire()  # Wait if buffer is full
        mutex.acquire()  # Enter critical section

        # Produce item in buffer
        buffer[in_index] = item
        print(f"Producer {producer_id} produced {item} at buffer[{in_index}]")
        in_index = (in_index + 1) % BUFFER_SIZE
        mutex.release()  # Exit critical section
        full.release()  # Signal that an item has been produced

        time.sleep(random.uniform(0.5, 1.5))  # Simulate production time


# Consumer function
def consumer(consumer_id):
    global out_index
    while True:
        full.acquire()  # Wait if buffer is empty
        mutex.acquire()  # Enter critical section

        # Consume item from buffer
        item = buffer[out_index]
        print(f"Consumer {consumer_id} consumed {item} from buffer[{out_index}]")
        buffer[out_index] = None
        out_index = (out_index + 1) % BUFFER_SIZE

        mutex.release()  # Exit critical section
        empty.release()  # Signal that a slot is empty

        time.sleep(random.uniform(1, 2))  # Simulate consumption time


# Create producer and consumer threads
producer_threads = []
consumer_threads = []
for i in range(3):  # Create 3 producer threads
    t = threading.Thread(target=producer, args=(i + 1,))
    producer_threads.append(t)
    t.start()
for i in range(3):  # Create 3 consumer threads
    t = threading.Thread(target=consumer, args=(i + 1,))
    consumer_threads.append(t)
    t.start()
# Join threads (they will run indefinitely in this example)
for t in producer_threads:
    t.join()
for t in consumer_threads:
    t.join()
