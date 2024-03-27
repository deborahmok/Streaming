In this code file, there is several classes that can be used to simulate a fictitious streaming service which contains different kinds of content, namely movies and series. In addition, the streaming service tracks its users and their viewing habits.

There is a User class, a Content class and its derived Movie and Series classes, as well as a StreamService class that models the whole service and stores the Users and Content. There is parsing of the streaming service’s content and user database, from an input file which uses a custom format, via a CustomParser class. This class inherits from a base Parser class so that we can easily create and use other formats. There is a parser for an alternate input format, JSON, so that even if the custom parser working isn't retrieving. All of this is supported by inheritance and polymorphism.

This code also provides a simple form of content suggestions based on what the user just watched and the viewing history of other users.
