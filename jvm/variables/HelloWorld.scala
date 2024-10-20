object HelloWorld{
    def main(args: Array[String]){
        val name = "Piseth Tang"
        val age = 20
        
        // First way to use variables inside the print line function 
        println(name + " is " + age + " years old")	

        // age = 10 // Error: Can't reassign variables 
        // Second way to use variables inside the print line function 
        // String interpolation, but not type-safe. 
        println(s"$name is $age years old")    

        // For type-safe interpolation, use F-string
        // In this case, name is automatically converted to uppercase
        println(f"$name%s is $age%d years old")
                

        // Third way to use variables inside the print line function
        val formattedMessage = s"${name.capitalize} is $age years old"
        println(formattedMessage)


    }

}