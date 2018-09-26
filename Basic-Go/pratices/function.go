/*
* Created for Go functions design
*/
package main

import (
	"fmt"
	"Template"
)

func main() {
	var (
		a string = "You"
		b string = "Suck"
	)
	Template.TemplatePrintf(a, b);
	fmt.Println(a, b);
}

func stringSwap(x, y *string) (a string, b string) {
	var temp string = *x;
	*x = *y;
	*y = temp;
	return *x, *y;
}
