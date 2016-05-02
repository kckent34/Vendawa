//
//  LoginViewController.swift
//  DoctorApp
//
//  Created by Casey Kent on 2/7/16.
//  Copyright © 2016 Casey Kent. All rights reserved.
//

import UIKit

class LoginViewController: UIViewController
{
    
    
    let validEmail = "ckent"
    let validPassword = "ckent"
    @IBOutlet weak var spinner: UIActivityIndicatorView!
    
    @IBOutlet weak var emailTextField: UITextField!

    @IBOutlet weak var passwordTextField: UITextField!
    
    
    
    
    
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    

    @IBAction func signIn(sender: UIButton)
    {

    }
    

    
    
    
    
    override func shouldPerformSegueWithIdentifier(identifier: String, sender: AnyObject?) -> Bool
    {
        spinner.startAnimating()
        if identifier == "Show Main Page"{
            if emailTextField.text! == validEmail && passwordTextField.text! == validPassword {
                NSLog("Login Succesful")
                spinner.stopAnimating()
                emailTextField.resignFirstResponder()
                passwordTextField.resignFirstResponder()
                return true
                
            } else {
                NSLog("Failed")
                spinner.stopAnimating()
                emailTextField.resignFirstResponder()
                passwordTextField.resignFirstResponder()
                return false
            }
        
        } else { return false }
        
        
    }
    

    
    
    
}
