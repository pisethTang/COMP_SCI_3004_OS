!@/bin/bash 

# Check if a commit message was provided
if [- "$1"]
then
	echo "Error: No commit message provided."
	exit 1
fi 


# Add and stage all changes 
git add .


# Commit with the provided message 
git commit -m "$1"


# Push to remote repo.
git push origin Practical_2
