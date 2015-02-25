The directory doc contains instructions how to use Autotools to update InstRO. 


Style guidelines

Filenames

Filenemas are generally mixed case. That means a file starts with an uppercase letter and then uses cammel case.
For example PassFactory.h

Files should be named like the class it contains. Generally one should have one class per file, unless there is a very good reason for not to follow this guideline.


Code style

We use an indentation with only tab characters. With that users can decide to display source w.r.t. their personal preference (e.g. 2 vs. 4 spaces per indentation).

Namespaces are not indented. Accordingly a class definition starts at column 0.

Naming

Although we use namespaces to separate disstinct entities, we still reflect this in a classes name.
For example would a Pass which is specialized to work with Rose be named RosePass and put into the Rose namespace - LLVMPass accordingly.
