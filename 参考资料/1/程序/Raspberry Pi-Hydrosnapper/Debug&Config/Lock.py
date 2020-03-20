import multiprocessing as mp
import time

def job(v, num, l): #传入锁

    for _ in range(10):
        time.sleep(0.1)
        print(l.acquire())  #锁住共享变量
        v.value += num
        print(v.value)
        l.release()  #释放共享变量

def multicore():
    l = mp.Lock() #定义锁
    v = mp.Value('i', 0)
    p1 = mp.Process(target=job, args=(v, 1, l))  #传入锁
    p2 = mp.Process(target=job, args=(v, 3, l))
    p1.start()
    p2.start()
    p1.join()
    p2.join()

if __name__ == '__main__':
    multicore()