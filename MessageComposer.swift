//
//  MessageComposer.swift
//  DoctorApp
//
//  Created by Casey Kent on 2/19/16.
//  Copyright © 2016 Casey Kent. All rights reserved.
//

import Foundation
import MessageUI


class MessageComposer:NSObject,MFMessageComposeViewControllerDelegate {
    
    var recipient:String?
    var first:String?
    var last:String?
    var med:String?
    var type:Int?
    var machineLocation:String?
    var codeString:String?
//    var code = Int(arc4random_uniform(8)+1)*1000 + Int(arc4random_uniform(8)+1)*100 + Int(arc4random_uniform(8)+1)*10 + Int(arc4random_uniform(8)+1)
    
    // A wrapper function to indicate whether or not a text message can be sent from the user's device
    func canSendText() -> Bool {
        return MFMessageComposeViewController.canSendText()
    }
    
    // Configures and returns a MFMessageComposeViewController instance
    func configuredMessageComposeViewController() -> MFMessageComposeViewController {
        let messageComposeVC = MFMessageComposeViewController()
        messageComposeVC.messageComposeDelegate = self  //  Make sure to set this property to self, so that the controller can be dismissed!
        
        if(type == 0){
//        let code = Int(arc4random_uniform(8)+1)*1000 + Int(arc4random_uniform(8)+1)*100 + Int(arc4random_uniform(8)+1)*10 + Int(arc4random_uniform(8)+1)
//        
//        let codeString = String(code) + String("#")
        
        let jsonObject: [String:AnyObject] = [
            "first":first!,
            "last":last!,
            "med":med!,
            "id":codeString!
            
        ]
        let valid = NSJSONSerialization.isValidJSONObject(jsonObject)
        var jsonString:String
        if(valid) {
            let jsonData = try! NSJSONSerialization.dataWithJSONObject(jsonObject, options: NSJSONWritingOptions())
            jsonString = NSString(data: jsonData, encoding: NSUTF8StringEncoding) as! String
        } else{
            jsonString = ""
        }
        
       
  
        messageComposeVC.recipients = [recipient!]
        messageComposeVC.body = jsonString
            
        }else if(type == 1){
            let messageString = "Hello, " + first! + " " + last! + " " + "your code at: " + machineLocation! + " APM is: " + codeString! + " " + "for " + med!
            messageComposeVC.recipients  = [recipient!]
            messageComposeVC.body = messageString
        }
        
        
        return messageComposeVC
    }
    
    // MFMessageComposeViewControllerDelegate callback - dismisses the view controller when the user is finished with it
    func messageComposeViewController(controller: MFMessageComposeViewController, didFinishWithResult result: MessageComposeResult) {
        controller.dismissViewControllerAnimated(true, completion: nil)
    }
    
    
}
