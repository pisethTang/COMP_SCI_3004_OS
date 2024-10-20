package content 

import scala.concurrent.Future
import scala.concurrent.ExecutionContext.Implicits.global

object AsyncNonBlocking{
    def blockingFunction(x: Int): Int = {
        Thread.sleep(10000)
        x + 42
    }

    blockingFunction(5)     // blocking call 
    val meaningOflIfe = 42 // will wait for 10 seconds before evaluating 
    

    // asynchronous blocking call 
    def asyncBlockingFunction(x: Int): Future[Int] = Future{
        Thread.sleep(10000)
        x + 42
    }

    asyncBlockingFunction(5)
    val anotherMeaningOfLife = 43 // evaluates immediately


    def main(args: Array[String]): Unit = {

    }
}