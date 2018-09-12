# url - comprehensive URL parsing

# API
## representation
URLs are not purely character arrays: they're composed of some important parts:

- domains

- fragment

- IP version

- parameters

- password

- path

- port

- query

- scheme

- whether the scheme is a protocol

- username

In its string representation, a URL takes this form:

    [scheme:][//][user[:password]@][host[:port]][/path][;parameters][?query][#fragment]

## operations
- creation, optionally with an initialization string

- conversion to a string representation

- binding URLs A and B into a new URL


### binding process
*parent refers to URL A*

*child refers to URL B*

1. begin with a copy of the child

2. if the child has no scheme
  
  - use the parent's scheme
  
  - set the "scheme is protocol" flag if either parent or child's corresponding flag is true
  
  - if the child has no host
    
    - use the parent's host
    
    - if the child has no port
      
      - use the parent's port
    
    - if the child has no path, or the child's path appears to be relative (not prefaced by '/')
      
      - join the parent's path with the child's path
