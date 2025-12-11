-- Индекс на поле name таблицы authors
CREATE INDEX idx_authors_name ON authors(name);

-- Индекс на поле title таблицы books
CREATE INDEX idx_books_title ON books(title);

-- Индекс на поле borrow_date таблицы borrowed_books
CREATE INDEX idx_borrowed_books_borrow_date ON borrowed_books(borrow_date);