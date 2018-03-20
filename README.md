# JSONDB Lite

A simple in memory database for storing and querying JSON documents. Works with nested documents and complex types.

### Supported Operations

* PUT {} - puts a document into the database
* GET {query} - get all documents matching the query
* delete - removes a document

### Usage

Current implementation reads all data from an input file and outputs to console

Example (input.txt) :

```
add {"id":1,"last":"Doe","first":"John","location":{"city":"Oakland","state":"CA","postalCode":"94607"},"active":true}
add {"id":2,"last":"Doe","first":"Jane","location":{"city":"San Francisco","state":"CA","postalCode":"94105"},"active":true}
add {"id":13,"last":"Frost","first":"Jack","location":{"city":"Seattle","state":"WA","postalCode":"98204", "address":{"street":"verde", "apt":"apt210"}},"active":false}
get {"apt":"apt210","street":"verde"}
```

### Enhancements

* Add C++ APIs for put and get.
* Add a shell for interactive query.
* Improve query runtime.


### Build

CMake is used for the build process

```
g++ -std=c++11 -o out main.cpp
```

### Run

The documents and query needs to be in a file input.txt

```
./out
```

