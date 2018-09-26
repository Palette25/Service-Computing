/*
* Creating Varibles and const values
*/
package main

import "fmt"

func main() {
	var (
		a int = 9
		b bool = false
	)

	fmt.Println(a, b);

	var arr = []int{0, 5, 7, 2, 9};
	for i:=0; i<5; i++ {
		fmt.Printf("arr[%d]=%d\n", i, arr[i]);
	}

	fmt.Println(getAverage(arr, 5));
}

func getAverage(arr []int, size int) float32 {
	var sum int = 0;
	for i:=0; i<size; i++ {
		sum += arr[i];
	}
	return float32(sum) / float32(size);
}
