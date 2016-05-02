//
//  Machine.swift
//  DoctorApp
//
//  Created by Casey Kent on 2/8/16.
//  Copyright © 2016 Casey Kent. All rights reserved.
//

import Foundation

class Machine {
    
    var location:String = ""
    var medicines = Dictionary<String,Int>()
    let phoneNumber = "7176694132"
  
    
    
    func pillCount(medicine:String) -> Int?{
        if containsMedicine(medicine){
            return medicines[medicine]
        } else {
            return 0
        }
    }
    
    func containsMedicine(medicine:String) ->Bool{
        return medicines.keys.contains(medicine)
    }
    
    func fill(meds:[String:Int]){
        for med in meds{
            medicines[med.0] = med.1
        }
    }
    
    func setLocation(loc:String){
        location = loc
    }
    
    
    
    
}