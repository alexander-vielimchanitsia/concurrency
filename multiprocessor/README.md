
# Multiprocessor scheduling

* 1 (virtual) core == 1 kernel thread
 - means it uses 1 kernel thread as a core for our threads, so our threads can run on different kernel threads
* kernel thread num == cores num * threads per core num
* so as many threads can run simultaneously as in real hyperthreading processors 
