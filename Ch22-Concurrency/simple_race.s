.main 
# This is a critical section 
mov 2000(%bx), %ax # get the value at the address 
add $1, %ax        # increment it by 1 
mov %ax, 2000(%bx) # store it back 
halt 
