# Implementing locking with a single memory flag 
# As was discussed, this approach is incorrect and performs poorly.
.var flag 
.var count 

.main
.top


.acquire
mov flag, %ax # get flag
test $0, %ax  # if we get 0 back: lock is free!
jne .acquire  # if not, try again
mov $1, flag  # store 1 into flag


# Critical section
mov count, %ax # get the value at the address
add $1, %ax    # increment it 
mov %ax, count # store the new value back into the address


# release lock
mov $0, flag # set flag to 0


# see if we're still looping
sub $1, %bx
test $0, %bx
jgt .top 

halt 
