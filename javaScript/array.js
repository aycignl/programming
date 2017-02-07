/* 

Gonul AYCI
Bogazici University, Feb. 2017
url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array
*/

//create an array
var fruits = ['Apple', 'Banana', 'Kiwi'];
console.log(fruits, fruits[0]);

//index into an Array item
var first = fruits[0];
var last = fruits[fruits.length - 1];
console.log(first, ',', last);

//loop over an Array
fruits.forEach(function(item, index, array){
	console.log(item, index);
});

//add to the end of an Array
var newElement = fruits.push('Orange');
console.log('push: ' + fruits + '. length: ' + fruits.length);

//remove from the end of an Array
var rmv = fruits.pop();
console.log('pop: ' + fruits);

//remove from the front of an Array
var first = fruits.shift();
console.log('pop first element: ' + fruits);

//add to the front of an Array
var newLength = fruits.unshift('Strawberry');
console.log('push front: ' + fruits);

//find the index of an item in the Array
fruits[6] = 'Mango';
var pos = fruits.indexOf('Strawberry');
console.log('position: ' + pos + '. Index of: ' + Object.keys(fruits));

//remove an item by index position
var removedItem = fruits.splice(pos, 1);
console.log('remove acc to position: ' + removedItem);

//copy an Array
var copyArray = fruits.slice();
console.log(copyArray);

//check include a certain element or not
console.log(fruits.includes('Mango'));

//string representation of an array and its elements vie toString()
console.log(fruits.toString());