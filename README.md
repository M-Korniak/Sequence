# Sequence

The task is to implement a dynamically loadable library in the C programming language to handle sets of sequences with an equivalence relation. The elements in the sets are non-empty sequences composed of the numbers 0, 1, and 2. Sequences are represented as strings, where, for example, the sequence {0, 1, 2} is represented as the string "012". Names can be assigned to abstract classes. 

# Library Interface 

Types and functions to be provided by the library, are declared in the attached seq.h file within the task description. In this interface, a sequence is correctly represented as a non-empty string consisting of the characters '0', '1', or '2,' and it must be terminated with a null character. A valid name for an abstract class is also a non-empty string terminated with a null character. Detailed information about the library's functionality, especially regarding what constitutes an invalid parameter or specific behavior, should be inferred from the attached seq_example.c file, which is an integral part of the specification. The term "the set of sequences has not changed" implies that there has been no observable change in the state of the sequence set.

# Function: seq_t * seq_new(void);

- Creates a new empty set of sequences.
- Returns a pointer to the data structure.
- Returns NULL on memory allocation failure, and the function sets errno to ENOMEM.

# Function: void seq_delete(seq_t *p);

- Deletes the set of sequences and frees all associated memory.
- Does nothing if called with a NULL pointer.
- After this function, the passed pointer becomes invalid.

# Function: int seq_add(seq_t *p, char const *s);

- Adds the given sequence and all non-empty subsequences as its prefixes to the set of sequences.
- Returns 1 if at least one new sequence is added to the set.
- Returns 0 if the set remains unchanged.
- Returns -1 if any of the parameters are invalid or if memory allocation fails, and it sets errno accordingly (EINVAL or ENOMEM).

# Function: int seq_remove(seq_t *p, char const *s);

- Removes the given sequence and all sequences for which it is a prefix from the set.
- Returns 1 if at least one sequence is removed from the set.
- Returns 0 if the set remains unchanged.
- Returns -1 if any of the parameters are invalid, and it sets errno to EINVAL.

# Function: int seq_valid(seq_t *p, char const *s);

- Checks if the given sequence is in the set of sequences.
- Returns 1 if the sequence is in the set.
- Returns 0 if the sequence is not in the set.
- Returns -1 if any of the parameters are invalid, and it sets errno to EINVAL.

# Function: int seq_set_name(seq_t *p, char const *s, char const *n);

- Sets or changes the name of the abstraction class to which the given sequence belongs.
- The provided name should be copied since the string pointed to by n may not exist after this function call.
- Returns 1 if the abstraction class name is set or changed.
- Returns 0 if the sequence is not in the set of sequences or if the class name is not changed.
- Returns -1 if any of the parameters are invalid or if memory allocation fails, and it sets errno accordingly (EINVAL or ENOMEM).

# Function: char const * seq_get_name(seq_t *p, char const *s);

- Returns a pointer to the name of the abstraction class to which the given sequence belongs.
- The returned pointer may become invalid after changes to the set of sequences or the data structure.
- Returns NULL if the sequence is not in the set of sequences or if the abstraction class does not have a name. It sets errno to 0.

# Function: int seq_equiv(seq_t *p, char const *s1, char const *s2);

- Combines two abstraction classes represented by the given sequences into one class.
- If both classes have names, the new class receives the combined name.
- If only one class has a name, the new class takes that name.
- If both classes have the same name, it remains the name of the new class.
- Returns 1 if a new abstraction class is created.
- Returns 0 if no new class is created because the given sequences already belong to the same class or one of them is not in the set of sequences.
- Returns -1 if any of the parameters are invalid or if memory allocation fails, and it sets errno accordingly (EINVAL or ENOMEM).
