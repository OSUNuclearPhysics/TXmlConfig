#include "TXmlConfig.h"

#include "TH1F.h"
#include "vector"


// This is a template specialization for
// creating and getting TH1* directly from config
template <>
TH1* TXmlConfig::get( std::string p, TH1* hDefault ) const{
    string name = get<string>( p+":name", "hist_name" );
    string title = get<string>( p+":title", "title" );
    vector<double> bins = getVector<double>( p+":bins-x", {1, 0, 1} );
    return new TH1F( name.c_str(), title.c_str(), bins[0], bins[1], bins[2] );
}

void example(){
    // create a config object and immediately load the config file 
    TXmlConfig cfg( "example.xml" );

    // dump all key-value pairs 
    cout << cfg.dump() << endl;

    //get values from the tree
    // paths are formed with a "." between levels and ":" for attributes:
    cout << cfg.get<std::string>("Level0.Level1.Level2:name", "NA") << endl;

    // you can get out any basic type:
    cout << "getting an integer from attr1: " << cfg.get<int>( "Level0:attr1", 0 ) << endl;
    cout << "getting an float from attr2: " << cfg.get<int>( "Level0:attr2", 0 ) << endl;
    cout << "getting an bool from attr3 (with string value='true'): " << cfg.get<bool>( "Level0:attr3", 0 ) << endl;
    cout << "getting an bool from attr4 (with string value='false'): " << cfg.get<bool>( "Level0:attr4", 1 ) << endl;

    // make all the histograms in the "Histograms" node tree
    // Use the above template specialization to handle direct TH1* 
    // creation from name, title, bins-x
    for ( auto p : cfg.childrenOf( "Histograms" ) ){
        TH1 * h = cfg.get<TH1*>( p, nullptr );    
        if (!h) continue;
        cout << "Created histogram: " << h->GetName() << ", title=" << h->GetTitle() << endl;
    }
}