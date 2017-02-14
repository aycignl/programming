 /* 

Gonul AYCI
Bogazici University, Feb. 2017
url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array
https://www.youtube.com/watch?v=nAWVYFEzoY8&list=PL-osiE80TeTucQUM10Ezv4S7SVoFozLMK&index=2#t=0.878963
*/

//get reverse list with reverse()  
var numArray1 = [0, 1, 2, 3, 4, 5];
console.log("Reversed: " + numArray1.reverse());

//concatenate 2 arrays with concat()
var stringArray1 = ['g', 'n', 'l'];
var stringArray2 = ['c', 'm', 'p', 'e'];
console.log(stringArray1.concat(' from ', stringArray2));

//joins elements of an Array with join()
var numArray2 = [777, 13, 192, 0];
console.log('joined: ', numArray2.join());
console.log('joined with dat: ', numArray2.join('.'));

//returns a portion of an array
console.log('sliced: ', numArray1.slice(2, 5), ', ', numArray2.slice(numArray2.length - 3), ', ', numArray1.slice(-3));

//deleting elements
console.log('original: ', numArray1);
console.log('deleted: ', numArray1.splice(2, 1), ', ', numArray1.splice(3));

//sort()
console.log('original: ' + numArray1 + ', sorted: ' + numArray1.sort());
console.log('original: ' + stringArray1 + ', sorted: ' + stringArray1.sort());

//filter()
var stringArray3 = [1, 2, 3, null, 4, 5, 'gnl', undefined];
stringArray4 = stringArray3.filter(function(item, index, array){
	return item; // try return true;
});
console.log(stringArray4);

//reduce()
stringArray5 = stringArray1.reduce(function(preValue, currValue, index, array){
	return preValue + currValue;
});
console.log(stringArray5);



 