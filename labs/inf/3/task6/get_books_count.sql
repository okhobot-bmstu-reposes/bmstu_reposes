
SELECT users.name as user_name, COUNT(borrowed_books.book_id) as borrowed_books_count
FROM borrowed_books
JOIN users ON borrowed_books.user_id = users.id
GROUP BY users.id, users.name;