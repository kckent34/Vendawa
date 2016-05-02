//
//  MedicineSearchViewController.swift
//  DoctorApp
//
//  Created by Casey Kent on 2/8/16.
//  Copyright © 2016 Casey Kent. All rights reserved.
//

import UIKit

class MedicineSearchViewController: UIViewController, UITableViewDataSource, UITableViewDelegate, UISearchBarDelegate, NSXMLParserDelegate{

    @IBOutlet weak var searchBar: UISearchBar!
    
    @IBOutlet var tableView: UITableView!
    var xmlParser = NSXMLParser()
    var searchActive: Bool = false
    var filtered : [String] = []
    var medData:Set<String> = []
    let baseUrl = "http://pillbox.nlm.nih.gov/PHP/pillboxAPIService.php"
    let key = "key=QBD6NV3TTX"
    var passData = false
    var currentElement:String = ""
    var medCount = 0
    
    override func viewDidLoad() {
        super.viewDidLoad()
        tableView.delegate = self
        searchBar.delegate = self
        tableView.dataSource = self
        print(medData)
        print(medData.count)
        
        
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func searchBarTextDidBeginEditing(searchBar: UISearchBar) {
        searchActive = true;
    }
    
    func searchBarTextDidEndEditing(searchBar: UISearchBar) {
        searchActive = false;
    }
    
    func searchBarCancelButtonClicked(searchBar: UISearchBar) {
        searchActive = false;
    }
    
    func searchBarSearchButtonClicked(searchBar: UISearchBar) {
        searchActive = false;
    }
    
    
    func searchBar(searchBar: UISearchBar, textDidChange searchText: String) {
        medData.removeAll()
        refreshMedicines(searchText)
        print(medData)
        filtered = medData.filter({ (text) -> Bool in
            let tmp: NSString = text
            let range = tmp.rangeOfString(searchText, options: NSStringCompareOptions.CaseInsensitiveSearch)
            return range.location != NSNotFound
        })
        if(filtered.count == 0){
            searchActive = false;
        } else {
            searchActive = true;
        }
        
        self.tableView.reloadData()
    }
    
    
    func refreshMedicines(searchString:String)
    {
        let trimmed = searchString.stringByTrimmingCharactersInSet(NSCharacterSet.whitespaceCharacterSet())
        let temp:NSString = trimmed
        let urlString = (baseUrl + "?ingredient=" + temp.lowercaseString + "&" + "has_image=1" + "&" + key)
        let url = NSURL(string: urlString)!
        let rssUrlRequest:NSURLRequest = NSURLRequest(URL:url)
        let queue:NSOperationQueue = NSOperationQueue()
        
        NSURLConnection.sendAsynchronousRequest(rssUrlRequest, queue: queue) {
            (response, data, error) -> Void in
            self.xmlParser = NSXMLParser(data: data!)
            self.xmlParser.delegate = self
            self.xmlParser.parse()
        }
        
        
    }
    
    
    // Parsing delegate methods 
    
    func parser(parser: NSXMLParser, didStartElement elementName: String, namespaceURI: String?, qualifiedName qName: String?, attributes attributeDict: [String : String]) {
        
        if(elementName=="RXSTRING" && medCount < 10)
        {
            passData=true;
            medCount = medCount + 1
        }
    }
    
    func parser(parser: NSXMLParser, didEndElement elementName: String, namespaceURI: String?, qualifiedName qName: String?) {
        currentElement="";
        if(elementName=="RXSTRING")
        {
            passData=false;
        }
    }
    
    func parser(parser: NSXMLParser, foundCharacters string: String) {
  
        if(passData)
        {
            let temp = string.componentsSeparatedByString("-")
            let temp2:NSString = temp[0]
            medData.insert(temp2.stringByTrimmingCharactersInSet(NSCharacterSet.whitespaceCharacterSet()).lowercaseString)
            print(medData.count)
        }
    }
    
    func parser(parser: NSXMLParser, parseErrorOccurred parseError: NSError) {
        NSLog("failure error: %@", parseError)
    }
    
    func parserDidEndDocument(parser: NSXMLParser){
        medCount = 0
        dispatch_async(dispatch_get_main_queue(), { () -> Void in
            self.tableView.reloadData()
            
        })
    }
    

    func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        if(searchActive) {
            return filtered.count
        }
        return medData.count;
    }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCellWithIdentifier("Cell")! as UITableViewCell;
        if(searchActive){
            cell.textLabel?.text = filtered[indexPath.row]
        } else {
            cell.textLabel?.text = medData.first
            medData.removeFirst()
        }
        
        return cell;
    }
    
    func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        tableView.deselectRowAtIndexPath(indexPath, animated: true)
    }

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        
        if let cell = sender as? UITableViewCell{
            if let text = cell.textLabel?.text
            {
                if let msc = segue.destinationViewController as? APMSelectionViewController{
                    msc.medicine = text
                }
               
            }

        }
        
    }
    
    
    
    
    

    }


