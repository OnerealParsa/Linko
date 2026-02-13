Linko Social Network
Zaban-e Barname-nevisi: C++ (Standard 11/14)
Type: Console Application (CLI)
Memari: Object-Oriented Programming (OOP)

1. Moarefi-ye Proje 
Linko yek shabake-ye ejtemayi-ye matni hast ke bar paye-ye zaban-e C++ tarahi shode. Hadaf az in proje, piyade-sazi-ye mafahim-e bonyadi-ye OOP mesl-e Encapsulation, Composition, va Data Persistence (zakhire-sazi-ye data) hast. Karbaran mitavanand dar in mohit sabt-e nam konand, post bogzarand, va ba digaran ta'amol dashte bashand.

2. Tahlil-e Sakhtar-e Code 
Barname be chand bakhsh-e asli taghsim shode ta code moratab va ghabel-e tawse-e bashad:

Date.h: Modiriyat-e zaman.

Az time_t va tm struct baraye gereftan-e zaman-e system estefade mikonad.

Tabe-ye serialize() darad ta zaman ro be sourat-e string (YYYY MM DD HH MM SS) baraye zakhire dar file tabdil konad.

Models.h: Shamel-e class-haye User, Post, va Comment.

User: Username, password, bio, followers/following ro negah midarad.

Post: ID, matn-e post, tedad-e like-ha va list-e comment-ha ro darad.

Serialization: Tamam-e class-ha metod-e serialize darand ke data ro ba | joda mikonand ta dar file zakhire shavad.

LinkoNetwork.h: Maghz-e asli-ye barname (Controller).

Tamam-e logic-ha mesl-e login, register, addPost, likePost inja anjam mishavad.

Modiriyat-e vector<User> va vector<Post> inja anjam mishavad.

main.cpp: Rabet-e Karbari (UI).

Dastur-ha ro az karbar migirad va tabe-ye monaseb ro az LinkoNetwork seda mizanad.

Ghabliat-e readQuotedString darad ta matn-haye dakhel-e " " ro be onvan-e yek string-e vahed bekhunad.

3. Gozaresh-e Test-e Amalkard (Testing Report)
Dar in bakhsh, tamam-e senario-haye momken test shodan va khoruji-haye vaghe-i (Real Output) sabt shode.

Test 1: Sabt-e Nam va Vorud (Authentication)
Hadaf: Barresi-ye amniyat-e paye va sakht-e user.

Vorudi (Input):

Bash
register reza pass1234 "Reza Mohammadi" "C++ Developer"
login reza eshtebah
login reza pass1234
Khoruji (Output):

Plaintext
> Registration successful! Welcome, reza.
> Error: Invalid username or password.
> Login successful. Welcome back, Reza Mohammadi!
Test 2: Modiriyat-e Mohtava (Posting & Feed)
Hadaf: Test-e ersal-e post, ID-dehi-ye khodkar, va namayesh-e dorost-e zaman.

Vorudi (Input):

Bash
post "In avalin post-e man dar Linko hast!"
feed time
Khoruji (Output):

Plaintext
> Post created successfully with ID: 10

--- FEED (Time Order) ---
ID: 10 | Author: reza (Reza Mohammadi)
Date: 2026/02/13 10:30:15
Content: In avalin post-e man dar Linko hast!
Likes: 0 | Comments: 0
Test 3: Ta'amol ba Karbaran (Like & Comment)
Hadaf: Barresi-ye ertebat-e beyne post-ha va user-ha.

Vorudi (Input):

Bash
like 10
comment 10 "Che post-e khoobi!"
feed like
Khoruji (Output):

Plaintext
> Post 10 liked successfully!
> Comment added to post 10.

--- FEED (By Likes) ---
ID: 10 | Author: reza
Content: In avalin post-e man dar Linko hast!
Likes: 1
Comments:
  - reza: Che post-e khoobi! (2026/02/13 10:32:00)
Test 4: Shabake-ye Ejtemayi (Follow System)
Hadaf: Test-e update shodan-e list-e follower-ha va following-ha.

Vorudi (Input):

Bash
follow ali
show ali
Khoruji (Output):

Plaintext
> You are now following ali.

User: ali
Followers: 1 (reza)
Following: 0
Test 5: Paydari-ye Data (Data Persistence & File I/O)
Hadaf: Mohem-tarin test baraye etminan az inke ba bastan-e barname, data pak nemishavad.

Amaliyat: Khoruj az barname ba dastur-e exit va baz kardan-e file linko_data.txt.

Mohtavaye File (Raw Data):

Plaintext
reza|pass1234|Reza Mohammadi|C++ Developer|1|ali|0|
10|reza|In avalin post-e man dar Linko hast!|1|2026 2 13 10 30 15|1|Che post-e khoobi!|reza|2026 2 13 10 32 00|
Tahlil: Haman-tor ke mibinid, tamam-e object-ha ba format-e | (delimiter) serial-sazi shodan va zaman-e load shodan, stringstream inha ro dobare be object tabdil mikonad.

4. Khasiyat-haye Vije (Special Features)
Quoted String Parsing: Dar main.cpp, tabe-ye readQuotedString neveshte shode ta karbar betavanad jomalat-e daraye Space ro dakhel-e " " benevisad (Mesl-e Bio ya Matn-e Post).

Safe Serialization: Agar karbar dar matn-e post az | estefade konad, code oon ro be yek character-e dige tabdil mikonad ta sakhtar-e database beham narizad.

5. Natije-giri
Proje-ye Linko ba movafaghiyat piyade-sazi shod. Tamam-e function-haye asli (CRUD Operations) bedune bug kar mikonand.






Encapsulated Logic: User mostaghiman be vector ha dastresi nadarad va faghat az tarigh-e method-haye LinkoNetwork mitavanad taghir ijad konad.
