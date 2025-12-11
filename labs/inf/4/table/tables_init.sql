DROP TABLE IF EXISTS borrowed_books;
DROP TABLE IF EXISTS books;
DROP TABLE IF EXISTS users;
DROP TABLE IF EXISTS authors;

CREATE TABLE authors 
(
    id INT PRIMARY KEY NOT NULL,
    name VARCHAR(50) NOT NULL,
    birth_year INT NOT NULL
);

CREATE TABLE books 
(
    id INT PRIMARY KEY NOT NULL,
    title VARCHAR(50) NOT NULL,
    author_id INT NOT NULL,
    publication_year INT NOT NULL,
    genre VARCHAR(50) NOT NULL,
    FOREIGN KEY (author_id) REFERENCES authors(id)
);

CREATE TABLE users 
(
    id INT PRIMARY KEY NOT NULL,
    name VARCHAR(50) NOT NULL,
    registration_date DATE NOT NULL
);

CREATE TABLE borrowed_books 
(
    user_id INT NOT NULL,
    book_id INT NOT NULL,
    borrow_date DATE NOT NULL,
    return_date DATE,
    FOREIGN KEY (user_id) REFERENCES users(id),
    FOREIGN KEY (book_id) REFERENCES books(id)
);
