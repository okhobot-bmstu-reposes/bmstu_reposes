DROP TABLE IF EXISTS borrowed_books;
DROP TABLE IF EXISTS books;
DROP TABLE IF EXISTS users;
DROP TABLE IF EXISTS authors;

CREATE TABLE authors 
(
	id INT PRIMARY KEY,
	name VARCHAR(50),
	birth_year INT
);
CREATE TABLE books 
(
	id INT PRIMARY KEY,
	title VARCHAR(50),
	author_id INT,
	publication_year INT,
	genre VARCHAR(50),
	FOREIGN KEY (author_id) REFERENCES authors(id)
);
CREATE TABLE users 
(
	id INT PRIMARY KEY,
	name VARCHAR(50),
	registration_date DATE
);
CREATE TABLE borrowed_books 
(
	user_id INT,
	book_id INT,
	borrow_date DATE, 
	return_date DATE,
	FOREIGN KEY (user_id) REFERENCES users(id),
	FOREIGN KEY (book_id) REFERENCES books(id)
);
