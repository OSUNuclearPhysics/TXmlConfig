# TXmlConfig
An XML Configuration interface for ROOT and analysis.
TXmlConfig helps separate configuration from logic and analysis.

## Basic Usage
```c++
root -l 
.L TXmlConfig.h
// Now create a TXmlConfig object
TXmlConfig cfg( "example.xml" ); // or use .load(...)

// Prints the map of keys <-> values loaded from the config file
cfg.dump();
```

### Accessing config with basic types
```c++
// get a string
std::string v = cfg.get<std::string>( "path.to.node:attribute-name", "default value if node DNE" );

// get an integer, note default value should be of the same type
int i = cfg.get<int>( "path.to.node:attribute-name", 0 );

// get an float
float f = cfg.get<float>( "path.to.node:attribute-name", 0.0 );

// get an double
double f = cfg.get<double>( "path.to.node:attribute-name", 0.0 );

// get a boolean
// this will try to interpret first as strings like "true" or "false"
// if not it will evaluate the truthyness of the integer conversion of the node value
bool b = cfg.get<bool>( "path.to.node:attribute-name", false );
```


## Specializations
You can add more functionality easily. This makes it so that you can work with higher level types, according to your needs. For instance, lets make a specialization to get a ROOT TH1* histogram directly from the config:
```c++
// This is a template specialization for
// creating and getting TH1* directly from config
template <>
TH1* TXmlConfig::get( std::string p, TH1* hDefault ) const{
    string name = get<string>( p+":name", "hist_name" );
    string title = get<string>( p+":title", "title" );
    vector<double> bins = getVector<double>( p+":bins-x", {1, 0, 1} );
    return new TH1F( name.c_str(), title.c_str(), bins[0], bins[1], bins[2] );
}
```

Now we can use this with a config line like this:
```xml
<Histogram name="h1" title="Hist1;p_{T};counts" bins-x="50, 0, 1" />
```
via
```c++
TH1* h = cfg.get<TH1*>( "path.to.Histogram", nullptr );
```