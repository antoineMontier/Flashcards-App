# Flashcards-App

flash card app is a program to study using flashcard : with a question, an hidden hint and a hidden answer

## it's using files with a .flash extension : 
### flash files contains  packages names,
- for each package there is a question number
- in each package there are questions
- for each question there is an answer
- for each question there is an optionnal hint

## code marks : 

- the beginning of a package is `<p=package-name>`
- the end of a package is `<\p>`
- the beginning of question is `<Q>`
- the end of a question is `<\Q>`
- the question field starts with `<q>` and ends with `</q>`
- the hint field starts with `<h>` and ends with `</h>`
- the answer field starts with `<a>` and ends with `</a>`

## code example :

```
<p=package_number_one>

<Q>
<q>
where is France ?
</q>

<a>
Europe
</a>
</Q>

<Q>
<q>
when started the 1st world war
</q>

<h>
it ended in 1918
</h>

<a>
1914
</a>
</Q>

<Q>
<q>
what is sq(-20)
</q>

<h>
sq(-1) = i
</h>

<a>
sq(-20) = 2i*sq(5)
</a>
</Q>

</p>
```
