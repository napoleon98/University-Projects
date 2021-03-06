**Ναπολέων Παπουτσάκης 9170**

**Γιώργος Εφραίμ Παππάς 9124**

# Αναφορά Εργαστηρίου 2

**Βήμα 1ο**

**_Ερώτημα 1_** 

Στο πρώτο ερώτημα ανατρέξαμε στα σχετικά αρχεία (_config.ini_) και βρήκαμε τις βασικές παραμέτρους του υποσυστήματος μνήμης **_L1I size, L1D size, L2 size, associativity, cache line size_** με τιμές:

1. _L1I size = 32768bytes_
2. _L1D size = 65536bytes_
3. _L2 size = 2097152bytes_
4. _L1D assoc = 2_
5. _L1I assoc = 2_
6. _L2 assoc = 8_
7. _Cache line size = 64bytes_

**_Ερώτημα 2_**

**Τρέξαμε** τα 5 διαφορετικά _**benchmarks**_ και κρατήσαμε από το καθένα τις πληροφορίες: 
1. **Χρόνος εκτέλεσης** (_sim seconds_)
2. **_CPI_**
3. **_Total Miss Rates_** για τα _L1D, L1I, L2_ επίπεδα **_cache_**

Αυτές υπάρχουν στα αρχεία _stats.txt_ των αποτελεσμάτων του εκάστοτε _benchmark_.

Για διευκόλυνσή μας δημιουργήσαμε ένα αρχείο _script_results.ini_ με βάση το παράδειγμα _conf_script.ini_ που δίνεται στην εκφώνηση, σε συνδιασμό με το έτοιμο _script_ _read_results.sh_.

Τα **γραφήματα** που απεικονίζουν τις πληροφορίες αυτές είναι τα εξής:

![simSec](https://github.com/gpappasv/eikones/blob/master/Screenshot_6.png)

![cpi](https://github.com/gpappasv/eikones/blob/master/screenshotsssss/Screenshot_4.png)

![miss rate1](https://github.com/gpappasv/eikones/blob/master/screenshotsssss/Screenshot_5.png)

![miss rate2](https://github.com/gpappasv/eikones/blob/master/screenshotsssss/Screenshot_8.png)

![miss rate3](https://github.com/gpappasv/eikones/blob/master/screenshotsssss/Screenshot_7.png)

Όπως φαίνεται απο τα διαγράμματα, υπάρχει άμεση αναλογία του _cpi_ με το _simulation seconds_ καθώς ορίσαμε το καθένα να τρέξει τον ίδιο αριθμό εντολών. Βλέποντας τα αποτελέσματα του _sjeng_, παρατηρούμε ότι το _CPI_ του είναι αρκετά μεγαλύτερο σε σχέση με τα άλλα _benchmarks_. Αυτό εξηγείται αν δούμε ότι τα _L1D_ και _L2_ _miss rate_ είναι αρκετά υψηλά. Γενικά το _miss rate_ από μόνο του δεν είναι καθοριστικός παράγοντας, οπότε ανατρέξαμε στο αρχείο _stats.txt_ και διαπιστώσαμε ότι ο αριθμός των _L1D accesses_ καθώς και των _L2 accesses_ του _sjeng_ _benchmark_ είναι ο μεγαλύτερος (ενδεικτικά, έχει πάνω απο 80.000.000 _accesses_ στην _L1D_ ενώ τα άλλα benchmarks δεν έχουν ξεπεράσει τα 53.000.000), σε σύγκριση με τα υπόλοιπα _benchmarks_. Άρα καταλαβαίνουμε ότι ο αριθμός των _misses_ είναι ιδιαίτερα υψηλός, που σημαίνει ότι πρέπει ο επεξεργαστής να "ψάξει" πολλές φορές στα πιο αργά επίπεδα μνήμης για τις πληροφορίες που ζητάει, κάτι που θα του κοστίσει αρκετούς κύκλους. 

Αντίστοιχα, αναφερόμενοι στο γρηγορότερο _benchmark_ (_mcf_), βλέπουμε ότι τα _miss rates_ είναι χαμηλά σε σύγκριση με τα υπόλοιπα, εκτός απο το _L2 miss rate_, που, αν και είναι το 3ο υψηλότερο, δεν του κοστίζει σε κύκλους - χρόνο, καθώς από το αντίστοιχο _stats.txt_ διαπιστώσαμε ότι έχει τον μικρότερο αριθμό σε _L2 accesses_ (~50.000, με το _hmmer_ να ακολουθεί στα ~70.000 και όλα τα άλλα να ανεβαίνουν πάνω απο 700.000).

**_Ερώτημα 3_**

Τα διάφορα _components_ της μητρικής έχουν ένα ρολόι κοινό για να υπάρχει συγχρονισμός μεταξύ τους. Αυτό το ρολόι λέγεται _**system clock**_ και έχει τιμή τέτοια που ανταποκρίνεται στο πιο αργό _component_ ώστε όλα τα σήματα να μπορούν να φτάσουν με κάποιον συγχρονισμό. Ο επεξεργαστής ακούει επίσης σε αυτό το ρολόι, αλλά έχει και έναν πολλαπλασιαστή, έτσι καταφέρνει να έχει ένα αρκετά υψηλότερο χρονισμό απο το υπόλοιπο σύστημα, κάτι που είναι απαραίτητο διότι χρειάζεται να εκτελεί πολλές διεργασίες στη μονάδα του χρόνου. ([πηγή](https://cs.stackexchange.com/questions/32149/what-are-system-clock-and-cpu-clock-and-what-are-their-functions?fbclid=IwAR3lRSriaay9s4x38LKbHEzDBff-jl5hjPFLA1uNOBaXVxJ8SpWojnnVfMg))
Ανατρέχοντας στο αρχείο _**config**_ όπου έχουμε θέσει το _**cpu clock**_ σε _**2Ghz**_, βλέπουμε ότι οι μνήμες _L1i,L1d,L2_ όπως και ο επεξεργαστής έχουν το ρολόι του επεξεργαστή (`clk_domain=system.cpu_clk_domain`), ενώ η _DRAM_ και τα υπόλοιπα στοιχεία ακούνε στο _system clock_ (`clk_domain=system.clk_domain`).
Προσθέτοντας άλλον έναν επεξεργαστή, θα έχει την ίδια συμπεριφορά με τον πρώτο. Θα χρονίζεται δηλαδή στα _2Ghz_.

Ανοίγοντας τα αντίστοιχα αρχεία _stats.txt_ για κάθε _benchmark_ και στις 2 περιπτωσεις _cpu clock_ (1 και 2 _Ghz_ ) πήραμε τις τιμές των _**sim_seconds**_ και τις καταγράψαμε στο παρακάτω πινακάκι:

|cpu-clock|bzip|hmmer|libm|mcf|sjeng|
|---|---|---|---|---|---|
|1Ghz|0.161337|0.118453|0.262355|0.109125|0.704063|
|2Ghz|0.084159|0.059368|0.174662|0.055477|0.513541|
|||||||
|scaling|1.917|1.995|1.502|1.967|1.3709|

Γενικά όπως είπαμε η _DRAM_ χρονίζεται με το _system clock_ (όπως και τα υπόλοιπα μέρη του υπολογιστή), ενώ μόνο ότι έχει να κάνει με τον επεξεργαστή παίρνει το ρολόι του επεξεργαστή (δηλαδή ο ίδιος ο επεξεργαστής και οι μνήμες _cache_ ). Όπως βλέπουμε από τα παρακάτω πινακάκια (ερώτημα 1 - βήμα 2), τα _accesses_ καθώς και το _miss rate_ στην _L2_ μας δείχνουν ότι οι περιπτώσεις που το σύστημα _cache_ δεν μας καλύπτει και καταφεύγουμε στα κατώτερα επίπεδα μνήμης είναι ελάχιστα στις περιπτώσεις των _bzip,hmmer,mcf_. Λόγω αυτών των ελάχιστων περιπτώσεων, δεν καταφέρνουμε να πάρουμε καθαρό x2 scaling, ενώ στις περιπτώσεις των _libm,sjeng_, που τα _accesses_ και το _miss rate_ στην _L2_ είναι ιδιαίτερα αυξημένα, έχουμε αντίστοιχα ~1,5 και ~1,4 _scaling_, αφού καταφεύγουμε σε συστήματα εκτός του επεξεργαστή περισσότερες φορές.

**Βήμα 2**

**_Ερώτημα 1_**

**_BZIP:_**
Στο _bzip_ βλέπουμε απο το αρχείο _stats.txt_ ότι για το _default_ _configuration_ έχουμε τα εξής στοιχεία:

|_cpi_|_missRateL1D_|_missRateL1i_|_missRateL2_|_L1i Accesses_|_L1d Accesses_|_L2 Accesses_|
|-----|-------------|-------------|------------|--------------|--------------|-------------|
|1.683172|0.0148|0.000074|0.281702|10198809|52164486|714016|

Όπως φαίνεται, στην _**L1i**_ δεν έχουμε μεγάλο πρόβλημα, καθώς έχουμε περίπου το 1/5 των **προσβάσεων** σε σχέση με την _L1d_ και το _**miss rate**_ της _L1i_ είναι ιδιαίτερα χαμηλό. Στην _**L1d**_ ωστόσο έχουμε πάρα πολλά _**accesses**_ και αρκετά υψηλότερο **_miss rate_**. Όσον αφορά στην _**L2**_, έχουμε το πιο υψηλό _**miss rate**_ σε σχέση με τις άλλες δύο μνήμες, αλλα έχουμε μικρό αριθμό _**accesses**_. Έτσι καταλαβαίνουμε ότι για να βελτιώσουμε το _cpi_ από αυτό το _benchmark_ πρέπει να εστιάσουμε πρώτα στην _**L1d**_ (και στο _cacheline size_) και ύστερα στην **_L2_** διότι απο τη μία έχουμε σε καθαρό αριθμό _misses_ το 1/7 (_misses_=_missRate_ x _NumOfAccesses_) αυτών της _L1i_ αλλά από την άλλη αυτά της _L2_ κοστίζουν πολύ περισσότερο από αυτά της _L1i_.

**_HMMER:_**
Στο _hmmer_ παίρνουμε απο το _stats.txt_ τα εξής:

|_cpi_|_missRateL1D_|_missRateL1i_|_missRateL2_|_L1i Accesses_|_L1d Accesses_|_L2 Accesses_|
|-----|-------------|-------------|------------|--------------|--------------|-------------|
|1.187362|0.001645|0.000205|0.082246|17142404|43865006|70496|

Εδώ βλέπουμε στην _**L1i**_ ότι έχει περίπου το 1/3 των _**accesses**_ σε σχέση με την _L1d_ και αρκετά χαμηλότερο _**miss rate**_, επομένως μεταξύ των 2 αυτών μεγαλύτερη βαρύτητα έχει η _**L1d**_. Παράλληλα, η _**L2**_ έχει το υψηλότερο _**miss rate**_ μεταξύ αυτών αλλά έχει πολύ χαμηλό αριθμό _**accesses**_. Οπότε για να βελτιώσουμε το _cpi_ από αυτό το _benchmark_ θα πρέπει να εστιάσουμε τα _test_ μας σε ότι έχει να κάνει με την _**L1d**_ και το _cacheline size_.

**_LIBM:_**
Αντίστοιχα για το _libm_ βλέπουμε τα εξής:

|_cpi_|_missRateL1D_|_missRateL1i_|_missRateL2_|_L1i Accesses_|_L1d Accesses_|_L2 Accesses_|
|-----|-------------|-------------|------------|--------------|--------------|-------------|
|3.493232|0.060971|0.000098|0.999927|5959112|48806561|1488553|

Από αυτά τα στοιχεία μας τραβάει αμέσως το ενδιαφέρον το γεγονός ότι η _**L2**_ έχει σχεδόν 1 _**miss rate**_, άρα όσες πληροφορίες φεύγουν από την _L1_ και πάνε στην _L2_ δεν ξαναχρησιμοποιούνται στο άμεσο μέλλον. Επίσης βλέπουμε ότι το _**miss rate**_ της _**L1d**_ είναι αρκετά υψηλότερο από αυτό της _L1i_ και τα _**accesses**_ της είναι όπως και στα προηγούμενα _benchmark_ πολλά. Επομένως πρέπει να εστιάσουμε τα _test_ μας στην _L1d_ στην _L2_ και στα _cacheline sizes_.

**_MCF_:**

|_cpi_|_missRateL1D_|_missRateL1i_|_missRateL2_|_L1i Accesses_|_L1d Accesses_|_L2 Accesses_|
|-----|-------------|-------------|------------|--------------|--------------|-------------|
|1.109538|0.002051|0.000037|0.72404|27009248|35173755|53631|

Σε αυτό το _benchmark_ βλέπουμε παρόμοια συμπεριφορά με το _hmmer_ αλλά το _miss rate_ της _L2_ εδώ είναι μεγαλύτερο. Άρα θα εστιάσουμε στην _L1d_, στην _L2_ και στα _cacheline sizes_.

_**SJENG**_ : Για το _sjeng_ απο το αρχείο _**stats.txt**_ βλέπουμε ότι το _default configuration_ έχουμε τα παρακάτω στοιχεία:

|_cpi_|_missRateL1D_|_missRateL1i_|_missRateL2_|_L1i Accesses_|_L1d Accesses_|_L2 Accesses_|
|-----|-------------|-------------|------------|--------------|--------------|-------------|
|10.27081|0.121829|0.00002|0.999979|31871080|86382246|5263951|

Παρατηρούμε ότι το _**miss rate**_ της _**L1i**_ είναι ιδιαίτερα χαμηλό , και σε συνδιασμό με τα _**accesses**_ της _L1i_ που ειναι 3 φορές (περίπου) λιγότερα σε σχέση με τα αντίστοιχα της _L1d_ συμπεραίνουμε οτι η πρώτη δεν επιβαρύνει, παρα ελάχιστα την απόδοση του συστήματος μας. Στην _**L1d**_ ο αριθμός των _**accesses**_ είναι πολύ μεγάλος και από το _**miss rate**_ για την _L1d_ προκύπτει ότι ενα ποσοστό της τάξης του 12% των _accesses_ καταλήγει σε _miss_, γεγονός που επιβαρύνει αρκετά το _**cpi**_. Σχετικά με την _L2_ βλέπουμε ότι το _**miss rate**_ της και ο αριθμός των _**accesses**_ είναι αρκετά μεγάλα. Έτσι, αντιλαμβανόμαστε ότι για τη βελτίωση της απόδοσης αυτού του _benchmark_ τα _tests_ θα πρέπει να εστιάσουν στη βελτίωση της _L1d_ της _L2_ και του _cacheline size_.

**_Ερώτημα 2_**

Στο ερώτημα 2 τρέξαμε κάθε _benchmark_ πειράζοντας κάθε φορά έναν παράγοντα για κάποιες τιμές που μπορεί αυτός να πάρει, ώστε να δούμε πόσο επηρεάζει κάθε παράγοντας το εκάστοτε _benchmark_. Προέκυψαν τα εξής διαγράμματα:

![cls](https://github.com/gpappasv/eikones/blob/master/cachelinesizes.png)
![l1ds](https://github.com/gpappasv/eikones/blob/master/l1dsizes.png)
![l1is](https://github.com/gpappasv/eikones/blob/master/l1i-sizes.png)
![l1da](https://github.com/gpappasv/eikones/blob/master/l1dassoc.png)
![l1ia](https://github.com/gpappasv/eikones/blob/master/l1iassoc.png)
![l2s](https://github.com/gpappasv/eikones/blob/master/l2sizes.png)
![l2a](https://github.com/gpappasv/eikones/blob/master/l2a.png)

Όπως βλέπουμε απο το διάγραμμα _**L1I sizes**_ μόνο το _mcf_ φαίνεται να επηρεάζεται απο την αλλαγή του _l1i size_ (συγκεκριμένα έχει σχετικά με τις άλλες περιπτώσεις κακή απόδοση όταν το _size_ γινει 16kb) κάτι που εξηγείται απο το γεγονός ότι έχει αυξημένο αριθμό _accesses_ στην _l1i_.
Αντίστοιχα στο διάγραμμα _**L1i associativity**_ διαπιστώνουμε ότι όλα τα _benchmarks_ δεν αλλάζουν συμπεριφορά σε σημαντικό βαθμό πειράζοντας κάτι που έχει να κάνει με την μνήμη _L1i_ και όντως είχαμε δει στην προηγούμενη ανάλυση του κάθε _benchmark_ ότι το _L1i_ έχει μικρό αντίκτυπο στο _cpi_ σε σχέση με τα επίπεδα _L1d_ και _L2_.

Βλέποντας τώρα τα διαγράμματα _**L1D sizes**_ και _**L1D associativity**_ διαπιστώνουμε ότι:

1. To _bzip_ βελτιώνεται αισθητά τόσο με αύξηση του μεγέθους αυτής της μνήμης όσο και με αύξηση του _associativity_ αυτό δηλώνει ότι το συγκεκριμένο _benchmark_ είχε πολλά _conflict misses_ καθώς και _capacity misses_.
2. Όλα τα υπόλοιπα _benchmarks_ δεν παρουσίασαν μεγάλες διαφορές στο _cpi_ τους πειράζοντας τη χωρητικότητα της _L1d_, άρα δεν είχαν σημαντικό αριθμό από _capacity misses_, όπως και με το _associativity_ δεν είδαμε μεγάλη διαφορά καθώς ανεβαίναμε πάνω από 2, άρα δεν έχουν σημαντικό αριθμό απο _L1d conflict misses_.

Στη συνέχεια, τα διαγράμματα _**L2 associativity**_ και _**L2 size**_ μας έδειξαν τα εξής:

1. Το _bzip_, όπως είχαμε αναφέρει και στο ερώτημα 1, επηρεάστηκε από τους παράγοντες _L2 size_ και _L2 associativity_, καθώς όταν αυξήσαμε το _size_ είδαμε σημαντική βελτίωση ενώ με συσχέτιση 2 και κάτω είχαμε πτώση του _cpi_.
2. Το _hmmer_ όπως αναμέναμε δεν επηρεάστηκε απο αλλαγές σε ότι έχει να κάνει με την _L2_ μνήμη.
3. Το _libm_, το _mcf_ και το _sjeng_ ενώ στην αρχή είχαμε δει κακή συμπεριφορά όσον αφορα τα _misses_ στην _L2_ και περιμέναμε με αλλαγές να δούμε κάποια αλλαγή στο _cpi_, έμειναν ανεπηρέαστα.

Τέλος, το _**Cache Line Size**_ μας έδειξε ότι μόνο το _hmmer_ δεν επηρεάστηκε σημαντικά από αλλαγές του _cacheline size_, τα _bzip_ και _mcf_ έδειξαν σημαντική βελτίωση σε ένα εύρος τιμών του _cacheline size_ ενώ το _libm_ και το _sjeng_ όσο αυξανόταν το _cacheline size_ τόσο άλλαζαν συμπεριφορά ως προς το _cpi_. Αυτό δείχνει ότι τα 2 τελευταία έχουν μεγάλο _locality_ και ότι το _hmmer_ δεν έχει καθόλου ως προς τα δεδομένα που ζητάει.

Έχοντας τα όσα είπαμε παραπάνω στο μυαλό μας, ξεκινήσαμε θέτοντας την κάθε παράμετρο μεμονομένα για το κάθε _Benchmark_ σύμφωνα με την βέλτιστη τιμή από τα _test_ που κάναμε για να δούμε την επίδραση του κάθε παράγοντα ξεχωριστά. Ύστερα πειράζοντας τις τιμές που πιστεύαμε ότι το εκάστοτε _benchmark_ ήταν πιο ευαίσθητο (όπως αναλύσαμε στο πρώτο ερώτημα), διαπιστώσαμε ότι τα βέλτιστα _configurations_ ήταν αυτά που φαίνονται στον πίνακα που ακολουθεί:

|Optimal|bzip|hmmer|libm|mcf|sjeng|
|--|--|--|--|--|--|
|l1i size|16|128|128|128|	128|
|l1d size	|128	|		  128		|	128		|	128		|	32|
|l2 size	|  4			|    4		|	  4		|	  4			|  4|
|l1i assoc	|4		|	    32|			32|			2|		  1|
|l1d assoc	|128			|  32		|	32	|		4			|  16|
|l2 assoc|	4		|	    1		|	  1	|		  4		|	  128|
|cacheline|	256			|  256		|	2048|		128	|		2048|
|cpi	|    1.585895|	1.179202	|1.495376|	1.083586|	3.071914|

**Βήμα 3**

Σε πρώτη φάση κατά την κατασκευή της συνάρτησης κόστους προσπαθούμε να βρούμε μια συσχέτιση μεταξύ του κόστους των **μεγεθών** των μνημών _L1_ και _L2_. Με βάση τη σύγκριση που γίνεται [εδώ](https://www.diffen.com/difference/Dynamic_random-access_memory_vs_Static_random-access_memory?fbclid=IwAR3QpZk02Gn5Mt0Z2vWhNkEhB-xXMqbGh4CNx5dKvLBeU8AV2v9DczFh7Sg#Price) μεταξύ _ram_ και _cache_ μνημών, που αναφέρει ότι η μνήμη _cache_ είναι περίπου κατά 100 φορές πιο ακριβή από την _DRAM_, καθώς και τις τιμές των _latency_ που βλέπουμε από ένα τυχαίο _benchmark_ σε πραγματικό υπολογιστή στην εικόνα που ακολουθεί (1) (που δείχνει την _l1_ με _latency_ _~1ns_, την _l2_ με _~3ns_ και την _DRAM_ με _~100ns_), αλλά και βάσει του ότι η _l1_ για λόγους απόδοσης πρέπει να είναι πιο κοντά στους πυρήνες, πράγμα που αυξάνει το _complexity_ του κυκλώματος όπως αναφέρεται [εδώ](https://stackoverflow.com/questions/4666728/why-is-the-size-of-l1-cache-smaller-than-that-of-the-l2-cache-in-most-of-the-pro),
```
L1 is very tightly coupled to the CPU core, and is accessed on every memory access (very frequent).
Thus, it needs to return the data really fast (usually within on clock cycle). Latency and throughput (bandwidth) are both performance-critical for L1 data cache. (e.g. four cycle latency, and supporting two reads and one write by the CPU core every clock cycle). It needs lots of read/write ports to support this high access bandwidth. Building a large cache with these properties is impossible. Thus, designers keep it small, e.g. 32KB in most processors today.

L2 is accessed only on L1 misses, so accesses are less frequent (usually 1/20th of the L1).

Thus, L2 can have higher latency (e.g. from 10 to 20 cycles) and have fewer ports. This allows designers to make it bigger.
```

θεωρούμε πως ένας συντελεστής 10 φορές μεγαλύτερος μπροστά από την _l1_ μνήμη είναι αντιπροσωπευτικός ως προς το "κόστος" της σε σχέση με την _l2_. 

(1) ![latency](http://www.overclock.net/photopost/data/1698200/0/00/00f46d0d_WTkZ0Xr.png) 

Στη συνέχεια, προσθέσαμε στην εξίσωση τους παράγοντες _l1i,l1d,l2 **associativity**_ καθώς και τον _**cacheline size**_ διότι όπως φαίνεται [εδώ](https://www.sciencedirect.com/topics/computer-science/set-associative-cache?fbclid=IwAR0BV52paxHl-guiW5IhZ6KgPm_z7ylSAV6bBJ0AIFZ0MDAykN2FevdDZLA) 
```
However, set associative caches are usually slower and somewhat more expensive to build because of the output multiplexer and additional comparators.
```
τα μεν _associativities_ προσθέτουν πολυπλοκότητα στο κύκλωμα καθώς και αύξηση του _hit time και miss penalty_ και το δε _cacheline size_ [αυξάνει το _hit time και miss penalty_](https://www3.nd.edu/~mniemier/teaching/2011_B_Fall/lectures/19_PPT_1up.pdf?fbclid=IwAR1AAsgqSh94S4XGyxw6N90OPS1V_b9njU8ggfY4_E1keWt0lWx3_aFYFcg) (7η σελίδα).

Από τις πηγές που έχουμε διαβάσει διαπιστώσαμε ότι η αύξηση του _size_ των μνημών κοστίζει περισσότερο από την αύξηση του _associativity_ , οπότε σαν συντελεστή κόστους βάζουμε μεν 150 μπροστά από τις τιμές των _l1i,l1d associativities_ και 100 στο _l2 associativity_ (καθότι βρίσκεται πιο μακριά από τον πυρήνα η _l2_ και σε πιο "εύκολο" σημείο απο το κύκλωμα του _l1 associativity_ ) αλλά το γινόμενο με την τιμή που θα δώσουμε στις αντίστοιχες μεταβλητές, είναι αρκετά χαμηλότερο σε σύγκριση με το γινόμενο που θα δώσει ο συντελεστής της _l1d size_, για παράδειγμα, με την αντίστοιχη μεταβλητή του, διότι τα sizes παίρνουν τιμές σε _kilobytes_ (32, 64, 128) ενώ οι τιμές των _associativities_ είναι 1,2,4,8... Έτσι η εξίσωση κόστους που προκύπτει είναι η εξής:

**Cost**=100*_**L1Isize**_ + 100*_**L1dsize**_ + 9*_**L2size**_ + 150*_**sqrt(L1Iassoc)**_ + 150*_**sqrt(L1dassoc)**_ + 100*_**sqrt(L2assoc)**_

Η παραπάνω εξίσωση λοιπόν μας λέει το τι πρέπει να πληρώσουμε από άποψη υλικού για το κάθε _configuration_. Η εξίσωση που ακολουθεί ωστόσο, συμπεριλαμβάνει και τη βελτίωση στην ταχύτητα του προγράμματος που προσφέρει το κάθε _configuration_ σε σχέση με το πιο φθηνό από αυτά. Δηλαδή μας λέει κατά πόσο _value for money_ είναι η κάθε επιλογή.

**BestValue**=Cost*(_CPInow_/_CPIlowestCost_)^5

Η δύναμη που βάλαμε στον όρο ( _CPINow_ / _CPIlowestCost_ ), εξυπηρετεί στην απόδοση μεγαλύτερης βαρύτητας στην επίδραση του _cpi gain_.

Στη συνέχεια προσπαθήσαμε να βρούμε το _optimal configuration_ με βάση το κόστος. Τρέξαμε λοιπόν τα _benchmarks_ με κάποια _configurations_,υπολογίσαμε τις τιμές των συναρτήσεων _**Cost**_ και _**BestValue**_ και τις παρουσιάζουμε στα παρακάτω διαγράμματα:

![bzip](https://github.com/gpappasv/eikones/blob/master/COSTBENCH/Screenshot_5.png)
![hmmer](https://github.com/gpappasv/eikones/blob/master/COSTBENCH/Screenshot_4.png)
![libm](https://github.com/gpappasv/eikones/blob/master/COSTBENCH/Screenshot_1.png)
![mcf](https://github.com/gpappasv/eikones/blob/master/COSTBENCH/Screenshot_3.png)
![sjeng](https://github.com/gpappasv/eikones/blob/master/COSTBENCH/Screenshot_2.png)

Από τα παραπάνω διαγράμματα κρατήσαμε λοιπόν τα _configurations_ που έχουν το καλύτερο _value_ (όπου καλύτερο = μικρότερο) για κάθε _benchmark_ και τα παραθέτουμε στον παρακάτω πίνακα:

|CostOptimal|l1i size|l1d size|l2 size|l1i assoc|l1d assoc|l2assoc|cacheline size|cpi|
|-|-|-|-|-|-|-|-|-|
|bzip|16kB|32kB|2MB|1|16|2|256|1.681863|
|hmmer|16kB|16kB|1MB|2|2|1|256|1.212773|
|libm|32kB|16kB|2MB|2|2|2|2048|1.496052|
|mcf|32kB|16kB|2MB|2|2|2|128|1.095878|
|sjeng|16kB|16kB|1MB|1|4|1|2048|3.091785|

Κριτική 2ου εργαστηρίου:

Η δομή του 2ου εργαστηρίου ήταν καλή ώστε να κατανοήσουμε καλύτερα τη λειτουργεία του gem5, καθώς και έννοιες όπως: _locality, cache associativity, block size_, αλλά και να γνωρίσουμε τους λόγους που η κρυφή μνήμη είναι μεν γρήγορη, αλλά ιδιαίτερα ακριβή. Παράλληλα  γνωρίσαμε τη δουλειά που κάνουν οι αρχιτέκτονες υπολογιστών, ως προς τη βελτιστοποίηση του _hardware_ για συγκεκριμένες λειτουργίες. Θεωρήσαμε όμως ότι το να έχουμε να διαχειριστούμε 5 _benchmarks_ πρόσθεσε περιττό φόρτο εργασίας, καθώς οι προσομοιώσεις που κληθήκαμε να τρέξουμε ήταν αρκετές σε αριθμό. Αν είχαμε να διαχειριστούμε μόνο 3 _benchmarks_ θα εμπεδώναμε στον ίδιο βαθμό τις έννοιες που χρειαζόταν και θα γλιτώναμε και κάποιον χρόνο. Η εκφώνηση πάντως ήταν πλήρης, χωρίς ασάφειες και κατατοπιστική.
