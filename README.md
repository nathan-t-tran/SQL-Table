# SQL-Table

//

This project represents a custom implementation of an SQL-like table system, developed entirely from scratch using Python. The core of the project lies in the integration of advanced data structures and algorithms, specifically the use of B+ trees to implement multimaps and maps. These structures are pivotal for efficiently handling the indexing and retrieval of data within the system.

At its foundation, this project includes a custom Table class that manages the creation, updating, deletion, and querying of data, much like a simplified SQL database. The B+ trees were crucial in optimizing the mapping of keys to multiple values, allowing for efficient range queries and data retrieval, especially when dealing with large datasets.

Tokenizers were implemented to parse and interpret user queries, breaking down the input into manageable tokens that the system can process. These tokenized inputs are then fed into a stack-based system for parsing and evaluating expressions, ensuring that the queries are handled accurately and in the correct order of operations. The stack and queue data structures were essential for managing the flow of data and commands within the system, enabling smooth processing of insertions, deletions, and complex query operations.

In essence, this project combines a variety of fundamental computer science principles, including the use of B+ trees for data indexing and retrieval, tokenizers for query interpretation, and stacks and queues for managing data flow. The result is a robust and efficient SQL-like table system that showcases the power of these foundational techniques when applied to database management.

//