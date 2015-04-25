# sqlite
SQLite binding for Cortex

# Setup

This project depends on a Cortex environment. Install [Cortex](https://github.com/cortexlang/cortex). Don't forget to `source configure` to ready the Cortex environment, and `make` it.


Install the sqlite3 development library:

```
sudo apt-get install libsqlite3-dev
```

Configure your Cortex-sqlite environment using the `configure` file in this project's directory.

```
source configure
```

Now, from this project's folder, run `make`.

## Test

Note: the testfiles should be created outside the sqlite directory.

The current workflow is

```
// testclasses.cx

enum DogBreed:: Golden, Shiba, Xoloitzcuintle

bitmask DogMood:: Cuddly, Playful, Kinky

struct Dog::
    name: string
    age: uint8
    breed: DogBreed
    mood: DogMood
```

```
// test1.cx store->db

sqlite::connector connector: "test.db"

string human: "Ada"::
    bool smart: true
    int64 trunk: 8::
        int32 leftArm: 4
        int32 rightArm: 4::
            int16 finger: 2
    uint64 rightLeg: 555

Dog johnny: "Johnny", 4, DogBreed::Shiba, DogMood::Cuddly|DogMood::Playful
Dog enrique: "Enrique", 10, DogBreed::Golden, DogMood::Playful|DogMood::Kinky|DogMood::Cuddly
```

```
// test2.cx db->store

sqlite::connector connector: "test.db"
```

And then run :

```
cortex testdef test1
cxsh testdef test2
```
