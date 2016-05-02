//
//  APMSelectionViewController.swift
//  DoctorApp
//
//  Created by Casey Kent on 2/8/16.
//  Copyright © 2016 Casey Kent. All rights reserved.
//

import UIKit

class APMSelectionViewController: UIViewController,UITableViewDataSource, UITableViewDelegate {
    
    var medicine:String?
    var machines: [Machine] = []
    var validMachines:[Machine] = []

    @IBOutlet weak var tableView: UITableView!
    @IBOutlet weak var displayLabel: UILabel!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        tableView.dataSource = self
        tableView.delegate = self
        
        if (medicine != nil) {
            displayLabel.text = displayLabel.text! + " " + medicine!
        }
        
        
        //set up 3 test machines for demo
        let m1:Machine = Machine()
        let m2:Machine = Machine()
        let m3:Machine = Machine()
        
        m1.fill(["ibuprofen":20 , "advil":30, "doxycycline":100])
        m1.setLocation("Philadelphia,PA")
        m2.fill(["ibuprofen":20 , "vyvanse":30, "doxycycline":100])
        m2.setLocation("Chicago,IL")
        m3.fill(["ibuprofen":60 , "morphine":50,"minocycline":10 ])
        m3.setLocation("Los Angeles,CA")
        
        machines.append(m1)
        machines.append(m2)
        machines.append(m3)
        
        findValidMachines()

        // Do any additional setup after loading the view.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    

    
    func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        print(validMachines.count)
        return validMachines.count
    }
    
    func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCellWithIdentifier("Cell")! as UITableViewCell
        cell.textLabel?.text = validMachines[indexPath.row].location
        
        if let pCount = validMachines[indexPath.row].pillCount(medicine!){
            cell.detailTextLabel?.text = String(pCount) + " blisters packs in stock"
        }

        return cell
    }
    
    
    func findValidMachines(){
        for m in machines{
            print(medicine!)
            if m.containsMedicine(medicine!){
                validMachines.append(m)
            }
        }
        tableView.reloadData()
    }
    
    
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?)
    {
        
        if let cell = sender as? UITableViewCell{
            if let text = cell.textLabel?.text
            {
                if let fovc = segue.destinationViewController as? FilloutViewController{
                    fovc.medicine = medicine
                    for machine in validMachines{
                        if machine.location == text { fovc.machine = machine }
                    }
                    
                    
                    
                }
                
            }
            
        }
    }

}

        
    
    

